module ReversibleNumbers where
import Data.Char (ord)

toDigits :: Int -> [Int]
toDigits number = [ord digit - ord '0'| digit <- show number]

isReversible :: Int -> Bool
isReversible n 
	| n `rem` 10 == 0 = False
	| otherwise = all odd (toDigits plus)
	where reversed = read $ reverse $ show n :: Int
	      plus = reversed + n

reversibleNumbers :: Int -> [Int]
reversibleNumbers n = [curr | curr<-[1..n], isReversible curr] 
