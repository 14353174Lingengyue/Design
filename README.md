## 蛋白质结构比对函数
main.cpp为蛋白质结构比对函数
编译
g++ main.cpp -o main
运行
./main map1 map2
map1和map2为SCOP蛋白质数据文件处理后的数据文件
## 数据处理程序
pdb2map.cpp为将SCOP蛋白质数据文件处理为map的程序
编译
g++ pdb2map.cpp -o pdb2map
运行
./pdb2map name.ent name.map
