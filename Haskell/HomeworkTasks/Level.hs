import Tree

level :: Int -> Tree a -> [a]
level lvl Empty = []
level 1 (Node value _ _) = [value]
level lvl (Node _ left right) = let next = pred lvl 
				in (level next left) ++ (level next right)
