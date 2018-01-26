function [] = HW_K4()
    F = @(x) (2+x)*cos(x)+x*(x+1);
    q = @(x) x+1;
    mu1 = 1;
    mu2 = 2*pi+1;
    a = 0;
    b = 2*pi;
    N = 17;
    h = (b - a) / N;
    [xout, yout] = ThomasAlgorithm(F, q, mu1, mu2, a, b, h);
    plot(xout, yout);
    title('Approximation')
    xlabel('x');
    ylabel('u(x)');
    
    [~, yout2] = ThomasAlgorithm(F, q, mu1, mu2, a, b, h/2);
    yout2 = yout2(1:2:end);
    [~, yout4] = ThomasAlgorithm(F, q, mu1, mu2, a, b, h/4);
    yout4 = yout4(1:4:end);
    rate=zeros(length(xout),1);
    for i = 1:length(xout)-1
        rate(i)=log2(abs((yout(i) - yout2(i))/(yout2(i) - yout4(i))));
    end
    plot(xout(1:end - 1), rate(1:end - 1), 'color', 'red');
    title('Rate of Convergence')
    xlabel('x');
    ylabel('alpha(x)');
    
    format long;
    fig = figure;
    uitable(fig, 'Data', [xout yout yout2 yout4 rate], 'Position', [20 20 260 204]);
end

function [xout, yout] = ThomasAlgorithm(F, q, mu1, mu2, a, b, h)
    N = round((b - a) / h);

    % allocating space
    alpha = zeros(N + 1, 1);
    beta  = zeros(N + 1, 1);
    xout  = zeros(N + 1, 1);
    yout  = zeros(N + 1, 1);
    
    % boundary rule
    xout(1) = a + 0*h;
    F0 = h*mu1 - F(xout(1))*(h^2)/2;
    c0 = 1 + (h^2)*q(xout(1))/2;
    b0 = 1;
    alpha(1) =  b0 / c0;
    beta(1)  = -F0 / c0;
    
    for i = 1:N-1
        xout(i+1) = a + i*h;
        Fi = -(h^2)*F(xout(i+1));
        ci = 2 + (h^2)*q(xout(i+1));
        bi = 1;
        ai = 1;
        alpha(i+1) =                 bi / (ci - ai*alpha(i));
        beta(i+1)  = (-Fi + ai*beta(i)) / (ci - ai*alpha(i));
    end
    
    % boundary rule
    xout(N+1) = b;
    FN = mu2;
    cN = -1;
    aN = 0;
    beta(N+1) = (-FN + aN*beta(N)) / (cN - aN*alpha(N));        
    
    yout(N+1) = beta(N+1);
    for i = N:-1:1
        yout(i) = alpha(i)*yout(i+1) + beta(i);
    end
end