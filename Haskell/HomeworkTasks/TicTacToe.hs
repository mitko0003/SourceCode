nextStates :: [[String]] -> String -> [[[String]]]
nextStates [] _ = []
nextStates state player = [createState row col| row <- [0..2], col <- [0..2],
						state !! row !! col == "-"]
	where createState row col = let prev = take row state
					new =  createRow (state !! row) col
					after = drop (succ row) state
				    in prev ++ (new: after)
	      createRow row col = (take col row) ++ (player: (drop (succ col) row))
