cd build
make 
cd ..
./bin/fla pda/case.pda "())()("
./bin/fla pda/case.pda "()(())"
./bin/fla pda/case.pda "(()()())"
./bin/fla pda/case.pda "()()(())())"

./bin/fla tm/case1.tm ab
./bin/fla tm/case1.tm aabb
./bin/fla tm/case1.tm aaabb
./bin/fla tm/case1.tm aaaabbb
./bin/fla tm/case1.tm abbba

./bin/fla tm/case2.tm 1
./bin/fla tm/case2.tm 111
./bin/fla tm/case2.tm 1111
./bin/fla tm/case2.tm 11111111
./bin/fla tm/case2.tm 111111111 