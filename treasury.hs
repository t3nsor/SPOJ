{- 2025-06-01
This solution was originally written in Python (see treasury.py). When that
solution got TLE because of the strict time limit, I had to rewrite it in a fast
language that has built-in bignums (I figured that my crappy bignum
implementation in C++ would also get TLE), so here we are. Though, the two are
not exactly the same, because here we avoid division. Maybe the Python one would
pass if I made that change to it, too. There are some ACs in Python. -}

import Data.Array
import Data.List

combine (p00, p01) (p10, p11)
  | p00 >  p10  =  (p00, p01)
  | p00 <  p10  =  (p10, p11)
  | p00 == p10  =  (p00, p01 + p11)

helper :: [((Integer, Integer), (Integer, Integer))] -> Integer

helper m = let cnts = map (\(r0, r1) -> (snd r0, snd r1)) m
               pps = scanl (*) 1 (map snd cnts)
               sps = tail (scanr (*) 1 (map snd cnts))
           in sum (zipWith3 (\x y z -> x*y*z) (map fst cnts) pps sps)

rec :: Array Int [Int] -> Int -> ((Integer, Integer), (Integer, Integer))

rec adj u =
    let cr = map (rec adj) (adj!u)
        p0 = (sum (map (fst . snd) cr),
              product (map (snd . snd) cr))
        diff = (\r -> fst (snd r) - fst (fst r))
        mindiff = minimum (2000:(map diff cr))
        (m, nm) = partition (\r -> diff r == mindiff) cr
        p1 = if 0 == length m
             then (0, 0)
             else (1 - mindiff + sum (map (fst . snd) cr),
                   product (map (snd . snd) nm) * (helper m))
    in (p0, combine p0 p1)

doCases :: [Integer] -> [[Int]] -> [Integer]

doCases results [] = reverse results

doCases results (line:lines) =
    let n = head line
        adj = map (map (-1+) . drop 2) (sortOn head (take n lines))
        result = rec (listArray (0,n-1) adj) 0
    in doCases ((snd $ snd $ result) : (fst $ snd $ result) : results)
               (drop n lines)

main = interact
       (unlines . map show . doCases [] . map (map read . words) . lines)
