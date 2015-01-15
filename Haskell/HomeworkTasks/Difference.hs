difference :: (Eq a) => [a] -> [a] -> [a]
difference list1 list2 = [num| num <- list1, not (elem num list2)]
