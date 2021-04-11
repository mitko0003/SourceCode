#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <algorithm>
#include <type_traits>
#include <functional>
#include <numeric>
#include <limits>
#include <memory>
#include <chrono>
#include <random>
#include <cstdint>
#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define BREAK __debugbreak()
#define ASSERT(condition) do { if(!(condition)) { printf("%s(%d): %s", __FILE__, __LINE__, #condition); BREAK; } } while(false)

///////////////////////////////
///////////////////////////////
// I hate globals, but to keep this simple, we'll have our image stuff be global
HDC         imageDC;        // the DC to hold our image
HBITMAP     imageBmp;       // the actual bitmap which contains the image (will be put in the DC)
HBITMAP     imageBmpOld;    // the DC's old bitmap (for cleanup)

const int   screenSize_X = 640;
const int   screenSize_Y = 480;

template<typename T>
T Square(const T& value)
{
	return value * value;
}

template<int32_t size>
struct GaussianBlur
{
	static_assert(size & 0b1, "Even kernel size!");
	float_t Kernel[size][size];

	// https://en.wikipedia.org/wiki/Multivariate_normal_distribution#Bivariate_case
	GaussianBlur(const float_t& mean, const float_t& variance)
	{
		const float_t mu = mean;
		const float_t sigma = variance;
		float_t sum = 0.0f;
		for (int32_t x = -size / 2; x <= size / 2; ++x)
		{
			for (int32_t y = -size / 2; y <= size / 2; ++y)
			{
				Kernel[x + size / 2][y + size / 2] = std::expf(-(Square(x - mu) + Square(y - mu)) / (2.0f * Square(sigma))) / (2.0f*float_t(M_PI)*Square(sigma));
				sum += Kernel[x + size / 2][y + size / 2];
			}
		}
		std::for_each(reinterpret_cast<float_t*>(Kernel), reinterpret_cast<float_t*>(Kernel) + size * size, [&sum](auto& value) -> void { value /= sum; });
	}

	GaussianBlur() : GaussianBlur(0.0f, 1.0f) {}
};

template<typename T, int32_t v>
struct Image
{
	T *data;
	int32_t w, h, c;

	Image() :
		data(nullptr), w(0), h(0), c(0) {}
	Image(const T &value, const int32_t &w, const int32_t &h, const int32_t &c) :
		data(new T[w * h * c]), w(w), h(h), c(c) {
		std::fill_n(data, w * h * c, value);
	}
	Image(const int32_t &w, const int32_t &h, const int32_t &c) :
		data(new T[w * h * c]), w(w), h(h), c(c) { memset(data, 0, w * h * c * sizeof(T)); }
	Image(const std::string &filename);
	~Image() { delete[] data; }

	template<typename T1, int32_t v1>
	explicit Image(const Image<T1, v1> &rhs)
	{
		w = rhs.w;
		h = rhs.h;
		c = rhs.c;
		data = new T[w * h * c];

		// TODO: add gamma correction
		/*const auto T1toT = [](T1 &value) -> T { 
			if (std::is_integral<T>::value) return std::powf(2.2);
		};*/
		for (int32_t idx = 0; idx < w * h * c; ++idx)
		{
			if (sizeof(T) > sizeof(T1))
				data[idx] = v * T(rhs.data[idx]) / v1;
			else
				data[idx] = v * T1(rhs.data[idx]) / v1;
		}
	}

	Image(Image&& rhs)
	{
		memcpy(this, &rhs, sizeof(Image));
		rhs.Image::Image();
	}

	Image(const Image& rhs)
	{
		Copy(rhs);
	}

	Image& operator=(const Image& rhs)
	{
		if (this != &rhs)
		{
			delete[] data;
			Copy(rhs);
		}
		return *this;
	}
	
	Image& operator=(Image&& rhs)
	{
		if (this != &rhs)
		{
			delete[] data;
			memcpy(this, &rhs, sizeof(Image));
			rhs.Image::Image();
		}
		return *this;
	}
	
	T operator^(const Image &rhs) const
	{
		ASSERT(w == rhs.w && h == rhs.h && c == rhs.c);
		T res = T(0);
		for (int32_t i = 0; i < w * h * c; ++i)
			res += data[i] * rhs.data[i];
		return res;
	}

	const T& operator[](const int32_t &index) const
	{
		return data[index];
	}

	T& operator[](const int32_t &index)
	{
		return data[index];
	}

	Image& operator-=(const Image& rhs)
	{
		ASSERT(w == rhs.w && h == rhs.h && c == rhs.c);
		for (int32_t idx = 0; idx < w * h * c; ++idx)
			data[idx] -= rhs.data[idx];
		return *this;
	}

	Image operator-(const Image& rhs) const
	{
		Image res(*this);
		return res -= rhs;
	}

	Image& operator+=(const Image& rhs)
	{
		ASSERT(w == rhs.w && h == rhs.h && c == rhs.c);
		for (int32_t idx = 0; idx < w * h * c; ++idx)
			data[idx] += rhs.data[idx];
		return *this;
	}

	Image operator+(const Image& rhs) const
	{
		Image res(*this);
		return res += rhs;
	}

	Image& operator+=(const T &rhs)
	{
		std::for_each(data, data + h * w * c, [&rhs](T &value) -> void { value += rhs; });
		return *this;
	}

	Image operator+(const T &rhs) const
	{
		Image res(*this);
		return res += rhs;
	}

	Image& operator*=(const Image& rhs)
	{
		ASSERT(w == rhs.w && h == rhs.h && c == rhs.c);
		for (int32_t idx = 0; idx < w * h * c; ++idx)
			data[idx] *= rhs.data[idx];
		return *this;
	}

	Image operator*(const Image& rhs) const
	{
		Image res(*this);
		return res *= rhs;
	}

	Image& operator*=(const T& rhs)
	{
		std::for_each(data, data + w * h * c, [&rhs](auto& value) -> void { value *= rhs; });
		return *this;
	}

	Image operator*(const T& rhs) const
	{
		Image res(*this);
		return res *= rhs;
	}

	template<int32_t size>
	Image<T, v> operator*(const GaussianBlur<size>& op) const
	{
		Image<T, v> res(w, h, c);

		for (int32_t k = 0; k < c; ++k)
		{
			for (int32_t i = 0; i < h; ++i)
			{
				for (int32_t j = 0; j < w; ++j)
				{
					float_t pixel = 0.0f;
					for (int32_t x = -size / 2; x <= size / 2; ++x)
					{
						for (int32_t y = -size / 2; y <= size / 2; ++y)
						{
							pixel += op.Kernel[x + size / 2][y + size / 2] * data[std::max(std::min(i + x, h - 1), 0) * w * c + std::max(std::min(j + y, w - 1), 0) * c + k];
						}
					}
					if (std::is_integral<T>::value)
						res.data[i * w * c + j * c + k] = std::roundf(pixel);
					else
						res.data[i * w * c + j * c + k] = pixel;
				}
			}
		}

		return res;
	}

	bool operator==(const Image& rhs) const
	{
		return w == rhs.w &&
			h == rhs.h &&
			c == rhs.c &&
			memcmp(data, rhs.data, w * h * c * sizeof(T)) == 0;
	}

	Image Tr() const
	{
		Image res(w, h, c);
		for (int32_t x = 0; x < h; ++x)
		{
			for (int32_t y = 0; y < w; ++y)
			{
				for (int32_t k = 0; k < c; ++k)
				{
					res.data[x * w * c + y * c + k] = data[y * w * c + x * c + k];
				}
			}
		}
		return res;
	}

	void Save(const std::string &filename) const;

private:
	void Copy(const Image& rhs)
	{
		w = rhs.w;
		h = rhs.h;
		c = rhs.c;
		data = new T[w * h * c];
		std::copy_n(rhs.data, w * h * c, data);
	}
};

template <>
Image<uint8_t, 255>::Image(const std::string &filename)
{
	auto tmp = stbi_load(filename.c_str(), &w, &h, &c, 0);
	data = new uint8_t[w * h];
	if (c == 3 || c == 4)
	{
		for (int32_t idx = 0; idx < w * h; ++idx)
		{
			float_t intensity =
				tmp[idx * c + 0] * 0.2989f +
				tmp[idx * c + 1] * 0.5870f +
				tmp[idx * c + 2] * 0.1140f;
			data[idx] = static_cast<uint8_t>(roundf(intensity));
		}
		c = 1;
	}
	else
	{
		ASSERT(c == 1);
		memcpy(data, tmp, w * h);
	}
	delete[] tmp;
}

template <>
void Image<uint8_t, 255>::Save(const std::string &filename) const
{
	stbi_write_bmp(filename.c_str(), w, h, c, data);
}

template<int32_t v>
Image<float_t, v> T(const Image<float_t, v>& image)
{
	auto res = image;
	std::for_each(res.data, res.data + res.w * res.h * res.c,
		[](auto& value) -> void {
			value = 2.0f * std::sqrtf(value + 3.0f / 8.0f);
		}
	);
	return res;
}

template<int32_t v>
float_t l2(const Image<float_t, v>& image)
{
	return std::accumulate(image.data, image.data + image.w * image.h * image.c, 0.0f, 
		[](const auto& sum, const auto& value) -> float_t {
			return sum + Square(value);
		}
	);
}

template<int32_t v>
float_t l1(const Image<float_t, v>& image)
{
	return std::accumulate(image.data, image.data + image.w * image.h * image.c, 0.0f,
		[](const auto& sum, const auto& value) -> float_t {
		return sum + std::fabs(value);
	}
	);
}

template<typename T, int32_t v>
using Grad = std::pair<Image<T, v>, Image<T, v>>;

template<int32_t v>
Grad<float_t, v> L(const Image<float_t, v>& image)
{
	Grad<float_t, v> res(
		std::piecewise_construct_t(), 
		std::make_tuple(image.w, image.h, image.c), 
		std::make_tuple(image.w, image.h, image.c)
	);

	for (int32_t i = 0; i < image.h - 1; ++i)
	{
		for (int32_t j = 0; j < image.w - 1; ++j)
		{
			res.first[i * image.w + j] = image.data[(i + 1) * image.w + j] - image.data[i * image.w + j];
			res.second[i * image.w + j] = image.data[i * image.w + j + 1] - image.data[i * image.w + j];
		}
	}

	return res;
}

template<int32_t v>
Image<float_t, v> L(const Grad<float_t, v> &grad)
{
	Image<float_t, v> res(grad.first.w, grad.first.h, grad.first.c);
	const auto &px = grad.first;
	const auto &py = grad.second;

	for (int32_t i = 0; i < res.h; ++i)
	{
		for (int32_t j = 0; j < res.w; ++j)
		{
			res[i * res.w + j] = (i - 1 >= 0 ? px.data[(i - 1) * res.w + j] : 0) - px.data[i * res.w + j] +
				(j - 1 >= 0 ? py.data[i * res.w + j - 1] : 0) - py.data[i * res.w + j];
		}
	}

	return res;
}

template<int32_t v>
Image<float_t, v> Pc(const Image<float_t, v>& image)
{
	auto res = image;
	std::for_each(res.data, res.data + res.w * res.h * res.c, [](auto& value) -> void { value = std::min(std::max(0.0f, value), float_t(v)); });
	return res;
}

template<int32_t v>
Image<float_t, v> Pv(const Image<float_t, v> &zeta, const float_t &tau)
{
	const auto dot = Image<float_t, v>(1.0f, zeta.w, zeta.h, zeta.c) ^ zeta;
	if (dot <= tau)
		return zeta;
	const float_t n = float_t(zeta.w * zeta.h * zeta.c);
	const auto lambda = (tau - dot) / n;
	return zeta + Image<float_t, v>(lambda, zeta.w, zeta.h, zeta.c);
}

template<typename T, int32_t v>
Image<T, v> P(const Image<T, v>& image)
{
	auto res = image;
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(static_cast<uint32_t>(seed));

	std::for_each(res.data, res.data + res.w * res.h * res.c,
		[&generator](auto& value) -> void {
			std::poisson_distribution<T> distribution(value);
			value = std::min(distribution(generator), T(v));
		}
	);

	return res;
}

template<int32_t v>
Grad<float_t, v> prox(const float_t &lambda, const Grad<float_t, v> &grad)
{
	auto res = grad;
	for (int32_t i = 0; i < res.first.w * res.first.h * res.first.c; ++i)
	{
		auto &x = grad.first[i];
		auto &y = grad.second[i];
		const auto scale = 1.0f - lambda / std::max(std::sqrtf(x*x + y*y), lambda);
		res.first[i] = x * scale;
		res.second[i] = y * scale;
	}
	return res;
}

//template<int32_t v>
//Image<float_t, v> prox(const float_t &lambda, const Image<float_t, v> &image)
//{
//	auto res = image;
//	std::for_each(res.data, res.data + res.w * res.h * res.c,
//		[&lambda](auto& x) -> void {
//			x = std::sign(x) * std::max(0, std::abs(x) - lambda));
//		}
//	);
//	return res;
//}

std::pair<float_t, float_t> Pepi(const float_t &z, const float_t &x, const float_t &zeta)
{
	const float_t t0 = 2.0f * std::sqrtf(std::max(x, 0.0f)) - z;

	const float_t phi = Square(t0);
	if (phi <= zeta)
		return std::pair<float_t, float_t>(std::max(x, 0.0f), zeta);
	else
	{
		const auto p = [&](const float_t &t) -> float_t { return 17.0f*t*t*t + 3.0f*z*t*t + (3.0f*z*z - 16.0f*zeta - 4.0f*x)*t + z*(z*z - 4.0f*x); };
		const auto dp = [&](const float_t &t) -> float_t { return 51.0f*t*t + 6.0f*z*t + 3.0f*z*z - 16.0f*zeta - 4.0f*x; };

		float_t diff = std::numeric_limits<float_t>::max();
		float_t t = t0;
		for (int32_t i = 0; i < 200 && std::fabs(diff) > 10e-5f; ++i)
		{
			diff = p(t) / dp(t);
			t -= diff;
		}

		return std::pair<float_t, float_t>(Square((t + z)/2.0f), Square(t));
	}
}

template<typename T, int32_t v>
float_t max(const Image<T, v>& image)
{
	return *std::max_element(image.data, image.data + image.h * image.w * image.c);
}

template<typename T, int32_t v>
float_t min(const Image<T, v>& image)
{
	return *std::min_element(image.data, image.data + image.h * image.w * image.c);
}

template<int32_t v>
float_t PSNR(const Image<float_t, v>& u, const Image<float_t, v>& f)
{
	ASSERT(u.w == f.w && u.h == f.h && u.c == f.c);
	const float_t n = u.w * u.h;
	return 10.0f * log10f(Square(max(u) - min(u))*n / l2(f - u));
}

template<int32_t v>
float_t MAE(const Image<float_t, v>& u, const Image<float_t, v>& f)
{
	ASSERT(u.w == f.w && u.h == f.h && u.c == f.c);
	const float_t n = u.w * u.h;
	return l1(u - f) / (n * v);
}

///////////////////////////////
///////////////////////////////
// Function to load the image into our DC so we can draw it to the screen
void loadImage(const Image<uint8_t, 255>& image)
{
	imageDC = CreateCompatibleDC(NULL);     // create an offscreen DC

	int8_t* tmp = static_cast<int8_t*>(alloca(image.w * image.h * 4));
	for (int32_t idx = 0; idx < image.w * image.h; ++idx)
	{
		tmp[idx * 4 + 0] = image.data[idx];
		tmp[idx * 4 + 1] = image.data[idx];
		tmp[idx * 4 + 2] = image.data[idx];
		tmp[idx * 4 + 3] = image.data[idx];
	}

	imageBmp = (HBITMAP)CreateBitmap(
		image.w,
		image.h,
		32, 1,
		tmp
		);

	imageBmpOld = (HBITMAP)SelectObject(imageDC, imageBmp);  // put the loaded image into our DC
}


///////////////////////////////
// Function to clean up
void cleanUpImage()
{
	SelectObject(imageDC, imageBmpOld);      // put the old bmp back in our DC
	DeleteObject(imageBmp);                 // delete the bmp we loaded
	DeleteDC(imageDC);                      // delete the DC we created
}

///////////////////////////////
///////////////////////////////
// The function to draw our image to the display (the given DC is the screen DC)
void drawImage(HDC screen)
{
	BitBlt(
		screen,         // tell it we want to draw to the screen
		0, 0,            // as position 0,0 (upper-left corner)
		screenSize_X,   // width of the rect to draw
		screenSize_Y,   // height of the rect
		imageDC,        // the DC to get the rect from (our image DC)
		0, 0,            // take it from position 0,0 in the image DC
		SRCCOPY         // tell it to do a pixel-by-pixel copy
		);
}


///////////////////////////////
///////////////////////////////
// A callback to handle Windows messages as they happen
LRESULT CALLBACK wndProc(HWND wnd, UINT msg, WPARAM w, LPARAM l)
{
	// what kind of message is this?
	switch (msg)
	{
		// we are interested in WM_PAINT, as that is how we draw
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC screen = BeginPaint(wnd, &ps);   // Get the screen DC
		drawImage(screen);                  // draw our image to our screen DC
		EndPaint(wnd, &ps);                  // clean up
	}break;

	// we are also interested in the WM_DESTROY message, as that lets us know when to close the window
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	// for everything else, let the default window message handler do its thing
	return DefWindowProc(wnd, msg, w, l);
}


///////////////////////////////
///////////////////////////////
// A function to create the window and get it set up
HWND createWindow(HINSTANCE inst)
{
	WNDCLASSEX wc = { 0 };        // create a WNDCLASSEX struct and zero it
	wc.cbSize = sizeof(WNDCLASSEX);     // tell windows the size of this struct
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));        // tell it to use the normal arrow cursor for this window
	wc.hInstance = inst;                   // give it our program instance
	wc.lpfnWndProc = wndProc;                // tell it to use our wndProc function to handle messages
	wc.lpszClassName = TEXT("DisplayImage");   // give this window class a name.

	RegisterClassEx(&wc);           // register our window class with Windows

									// the style of the window we want... we want a normal window but do not want it resizable.
	int style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;    // normal overlapped window with a caption and a system menu (the X to close)

															// Figure out how big we need to make the window so that the CLIENT area (the part we will be drawing to) is
															//  the desired size
	RECT rc = { 0,0,screenSize_X,screenSize_Y };      // desired rect
	AdjustWindowRect(&rc, style, FALSE);              // adjust the rect with the given style, FALSE because there is no menu

	return CreateWindow(            // create the window
		TEXT("DisplayImage"),       // the name of the window class to use for this window (the one we just registered)
		TEXT("Display an Image"),   // the text to appear on the title of the window
		style | WS_VISIBLE,         // the style of this window (OR it with WS_VISIBLE so it actually becomes visible immediately)
		100, 100,                    // create it at position 100,100
		rc.right - rc.left,         // width of the window we want
		rc.bottom - rc.top,         // height of the window
		NULL, NULL,                  // no parent window, no menu
		inst,                       // our program instance
		NULL);                      // no extra parameter

}

///////////////////////////////
///////////////////////////////
// The actual entry point for the program!
//  This is Windows' version of the 'main' function:
// int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
int main()
{
	// Create noisy image
	const std::string Images[] = { "boats", "cameraman", "lena", "peppers" };
	for (auto const &ImageName: Images)
	{
		Image<uint8_t, 255> B1(ImageName + ".png");
		constexpr float_t gauss_mu = 0.0f;
		constexpr float_t gauss_sigma = 1.3f;
		constexpr float_t fN = 3.0f * gauss_sigma;
		constexpr int32_t kernel_width = float_t(int32_t(fN)) == fN ? 2 * int32_t(fN) + 1 : 2 * int32_t(fN) + 3;
		GaussianBlur<kernel_width> H(gauss_mu, gauss_sigma);

		constexpr int32_t Intensity = 255;
		constexpr bool OutputStatistics = true;
		constexpr int32_t MaxIterations = 300;
		
		const auto original = Image<float_t, Intensity>(B1);
		const Image<float_t, Intensity> f(P(original*H));
		auto u = f;
		const auto Tf = T(f);
		const float_t tau = float_t(u.w * u.h);
		Image<uint8_t, 255>(f).Save("progress/" + ImageName + "Corrupt.bmp");
		const float_t sigma = 0.2f;
		const float_t ro = 0.5f;
		const float_t theta = 1.0f;
		const auto n = u.w * u.h * u.c;
		Image<float_t, Intensity> zeta(u.w, u.h, u.c);
		Image<float_t, Intensity> eta(u.w, u.h, u.c);
		Image<float_t, Intensity> v1(u.w, u.h, u.c);
		Grad<float_t, Intensity> v2(
			std::piecewise_construct_t(),
			std::make_tuple(u.w, u.h, u.c),
			std::make_tuple(u.w, u.h, u.c)
			);
		Image<float_t, Intensity> p1(u.w, u.h, u.c);
		Image<float_t, Intensity> p3(u.w, u.h, u.c);
		Image<float_t, Intensity> p1_(u.w, u.h, u.c);
		Image<float_t, Intensity> p3_(u.w, u.h, u.c);
		Grad<float_t, Intensity> p2(
			std::piecewise_construct_t(),
			std::make_tuple(u.w, u.h, u.c),
			std::make_tuple(u.w, u.h, u.c)
			);
		Grad<float_t, Intensity> p2_(
			std::piecewise_construct_t(),
			std::make_tuple(u.w, u.h, u.c),
			std::make_tuple(u.w, u.h, u.c)
			);

		struct
		{
			float_t fidelity;
			float_t psnr;
			float_t mae;
		} stats[MaxIterations];

		const auto ComputeStats = [&](const int32_t &idx) -> void
		{
			if (OutputStatistics)
			{
				const auto n = float_t(u.w * u.h);
				stats[idx].fidelity = l2(Tf - T(u)) / n;
				stats[idx].psnr = PSNR(original, u);
				stats[idx].mae = MAE(original, u);
			}
		};

		ComputeStats(0);
		for (int32_t k = 0; k < MaxIterations; ComputeStats(++k))
		{
/*1*/		u = Pc(u - (p1_*H + L(p2_))*ro*sigma);
/*2*/		zeta = Pv(zeta - p3_*ro*sigma, tau);

			const auto Hu = u*H;
			for (int32_t i = 0; i < n; ++i)
			{
/*3*/			const auto res = Pepi(Tf[i], p1[i] + Hu[i] + 3.0f / 8.0f, p3[i] + zeta[i]);
				v1[i] = res.first;
				eta[i] = res.second;
			}
			
			const auto Lu = L(u);
/*4*/		v2 = prox(1.0f / sigma, Grad<float_t, Intensity>(p2.first + Lu.first, p2.second + Lu.second));

/*5*/		const auto p1_diff = Hu - v1 + 3.0f / 8.0f;
/*6*/		const auto p2_diff = Grad<float_t, Intensity>(Lu.first - v2.first, Lu.second - v2.second);
/*7*/		const auto p3_diff = zeta - eta;

			p1 = p1 + p1_diff;
			p2 = Grad<float_t, Intensity>(p2.first + p2_diff.first, p2.second + p2_diff.second);
			p3 = p3 + p3_diff;
/*8*/		p1_ = p1 + p1_diff*theta;
			p2_ = Grad<float_t, Intensity>(p2.first + p2_diff.first*theta, p2.second + p2_diff.second*theta);
			p3_ = p3 + p3_diff*theta;

			if ((k + 1) % (MaxIterations / 10) == 0) printf("%s: %d0%%\n", ImageName.c_str(), (k + 1) / (MaxIterations / 10));
		}
		
		if (OutputStatistics)
		{
			FILE *pFile = fopen(("progress/" + ImageName + ".txt").c_str(), "w");
			for (int32_t k = 0; k < MaxIterations; ++k)
				fprintf(pFile, "%d: fidelity=%f psnr=%f mae=%f\n", k, stats[k].fidelity, stats[k].psnr, stats[k].mae);
			fclose(pFile);
		}

		Image<uint8_t, 255>(u).Save("progress/" + ImageName + "Fixed.bmp");
	}
	return 0;
}
