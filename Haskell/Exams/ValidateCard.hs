import ReversibleNumbers (toDigits)

isValid :: Int -> Bool
isValid number = (sumOfOdd + sumOfEven) `rem` 10 == 0  
	where digits = reverse $ toDigits number 
	      digitsCount = pred $ length digits
	      sumOfOdd = sum [digits !! index|index<-[0, 2 .. digitsCount]]
	      sumOfEven = sum [calcEven (digits !! index)|index<-[1, 3 .. digitsCount]]
	      calcEven n = sum $ toDigits (n * 2)
