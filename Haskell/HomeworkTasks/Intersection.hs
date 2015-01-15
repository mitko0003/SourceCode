intersection :: (Eq a) => [a] -> [a] -> [a]
intersection list1 list2 = [num| num <- list1, elem num list2]
