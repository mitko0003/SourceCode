import Data.Bits (xor)
import Tree

-- The function assumes that the two elements are in the tree
lca :: (Ord a) => a -> a -> Tree a -> a
lca first second (Node value left right)
	| first > value && second > value = lca first second right
	| first < value && second < value = lca first second left
	| otherwise = value
