data Tree a = Empty | Node a (Tree a) (Tree a)

fullTree = Node 1
        (Node 2
            (Node 3 Empty Empty)
            (Node 4 Empty Empty))
        (Node 5 Empty Empty)
nonFullTree = Node 1
            (Node 2
                (Node 3 Empty Empty)
                (Node 4 Empty Empty))
            Empty

isFull :: Tree a -> Bool
isFull (Node _ Empty Empty) = True
isFull (Node _ Empty _) = False
isFull (Node _ _ Empty) = False
isFull (Node _ left right) = (isFull left) && (isFull right)
