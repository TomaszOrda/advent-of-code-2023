A record of my solutions for 2023 [Advent Of Code](https://adventofcode.com/2023 "AoC").

I have not attended AoC in 2023. I have now (2025) decided to practice and relearn C++. It has been around six years since I last touched any C language.

Solutions for each day and task are in separate files.

To run the solution:
1. Build the designated solution file along with `../runner.cpp` and `../utils.cpp`. (Consult the .vscode folder in case of troubles.)
2. Default build and run task uses `test` parameter. It uses the puzzle input, when running executable without any parameter. The puzzle input file should be named the same as the test input, but without `_test` suffix.
3. Alternatively, you can run the solution with a custom test input file changing the main function accordingly (`test_run()` -> `run(path)`).

Data for each task can be accessed through the site.

All solutions but two run under a second on my machine. Day 23 part 2 completes in under 4 seconds, and Day 25 uses a custom heuristic, which may or may not be fast enough. I opted for a heuristic because I did not want to copy code from Wikipedia — which in non-challenge scenario would be the sane thing to do.

50/50⭐

|            | 日 | 月 | 火 | 水 | 木 | 金 | 土 |
|:-----------|-------|-------|-------|-------|-------|-------|-------|
| **Week 1** |⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|
| **Week 2** |⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|
| **Week 3** |⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|
| **Week 4** |⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;⭐|⭐&nbsp;🌟|🎄&nbsp;&nbsp;🎄|||

An estimated rundown of the time invested in each day puzzle (includes coding and coding-adjacent activities). Bar sections represent from the leftmost: first part solution, second part solution, solutions refinement and utilities.

```
Day  1 | ███████████████|████████████| 225
Day  2 | ████████|███████|██████ 174
Day  3 | ██████|█████|█████████ 165
Day  4 | ██████|█████| 91
Day  5 | ██████████████|█████████████████|████████ 320
Day  6 | ████|████|██ 86
Day  7 | ████████████|██| 116
Day  8 | █████████|███████████████████████████| 299
Day  9 | █████|█|█ 58
Day 10 | ███████████|████████|█████████ 232
Day 11 | ███|██|████ 72
Day 12 | ██████████|███|█████████ 185
Day 13 | ███████████|████| 121
Day 14 | █████|██████████|█ 135
Day 15 | ████|██████| 82
Day 16 | ████████|████| 102
Day 17 | ███████████|█|██████████████████████████████████ 385
Day 18 | ██████████████████████████████████|█|███ 317
Day 19 | ███████|██████████| 140
Day 20 | ███████████████████|████████████████████████████████| 427
Day 21 | █████|██████████████████████████████████████████████████████████|██ 539
Day 22 | ████████████████████████|█████████| 277
Day 23 | ████████|████████|████████████████ 265
Day 24 | █████████████|████████████████████| 272
Day 25 | ███████████████████████████████████████||███ 347
```