--Harry Pham
--79422112

--Question 1
insert::(Ord a) => [a] -> a -> [a]
insert [] elem = elem:[]
insert (x:xs)elem	| elem <= x = elem:(x:xs)
					| elem > x = x:(insert xs elem)

--Question 2
insertSort::(Ord a) => [a] -> [a]
insertSort [] = []
insertSort (x:xs) = insert (insertSort xs) x

--Question 3
merge::(Ord a) => [[a]] -> [a]
merge [] = []
merge (x:[]) = x
merge (x:xs) = insertSort (x ++ merge (xs))

--Question 4
createList::a -> Int -> [a]
createList a 0 = []
createList a l	| l < 0 = []
				| l > 0 = a:createList a (l - 1)
				
center::(Ord a) => [a] -> Int -> a -> [a]
center arg1 arg2 arg3	| length arg1 <= 0 = []
						| length arg1 == arg2 = arg1
						| not(length arg1 == arg2) = l++arg1++r
						where
							len = length arg1
							remaining = arg2 - len
							front = remaining `div` 2
							back = remaining - front
							l = createList arg3 front
							r = createList arg3 back
				
--Question 5
largest::(Ord a) => [a]->a
largest (x:[]) = x
largest (x:xs) = max x (largest xs)