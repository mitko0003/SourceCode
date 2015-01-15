import Tree

pathTo :: (Ord a) => a -> Tree a -> [a]
pathTo elem tree
	| path == [] = error "no path"
	| otherwise = path
	where path = findPath tree 
	      findPath Empty = []
	      findPath (Node value left right)
			| value == elem = [value]
			| leftPath /= [] = value: leftPath
			| rightPath /= [] = value: rightPath
			| otherwise = []
			where leftPath = findPath left 
			      rightPath = findPath right
