import Data.List (maximumBy)

data Answer = Yes Int | No Int | Unknown deriving (Show)
type Test = [Answer]
type Exam = [Test]

-- testScore -> 5 - 3 + 0 + 2 - 1 + 3 + 2 - 1 + 0 = 7
exampleTest1 :: Test
exampleTest1 = [Yes 5, No 3, Unknown, Yes 2, No 1, Yes 3, Yes 2, No 1, Unknown]
-- testScore -> 10
exampleTest2 :: Test
exampleTest2 = [Yes 5, Yes 5, Yes 5, No 5, Unknown]
--  averageScore -> 8.5
--  highestScore -> exampleTest2
exampleExam :: Exam
exampleExam = [exampleTest1, exampleTest2]

valueOfAnswer :: Answer -> Int
valueOfAnswer (Yes n) = n
valueOfAnswer (No n) = (-n)
valueOfAnswer Unknown = 0

testScore :: Test -> Int
testScore [] = 0
testScore answers = sum $ map valueOfAnswer answers

averageScore :: Exam -> Double
averageScore [] = 0
averageScore tests = (fromIntegral $ sum $ map testScore tests) / (fromIntegral $ length tests)

aboveLimit :: Int -> Exam -> Int
aboveLimit _ [] = 0
aboveLimit limit tests = length $ filter (>=limit) (map testScore tests) 

highestScore :: Exam -> Test
highestScore tests = (maximumBy compareFunc tests)
	where compareFunc x y
		| scoreX == scoreY = EQ
		| scoreX > scoreY = GT
		| otherwise = LT
		where scoreX = testScore x
		      scoreY = testScore y
