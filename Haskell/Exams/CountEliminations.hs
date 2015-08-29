import Data.List (elemIndices)

countEliminations :: [Int] -> Int
countEliminations numbers
	| eliminations == 0 = 0
	| otherwise = 1 + (countEliminations $ eliminate (head eliminatable) numbers)
	where indicesOfMax = elemIndices (maximum numbers) numbers
	      eliminatable = [(x,y)| (x,y)<-zip indicesOfMax (tail indicesOfMax), succ x == y]
	      eliminations = length eliminatable
	      eliminate (x, y) numbers = take x numbers ++ drop (succ y) numbers 	
