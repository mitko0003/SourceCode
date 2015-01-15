module Tree where

data Tree a = Empty | Node a (Tree a)(Tree a)

left :: Tree a -> Tree a
left Empty = Empty
left (Node _ l _) = l

right :: Tree a -> Tree a
right Empty = Empty
right (Node _ _ r) = r

value :: Tree a -> a
value Empty = error "Node is Empty!"
value (Node v _ _) = v

testTree :: Tree Int
testTree = Node 11
                (Node 7
                    (Node 4 Empty Empty)
                    (Node 9 Empty Empty))
                (Node 21 Empty Empty)
