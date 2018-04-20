## 蛋白质结构比对函数
main.cpp为蛋白质结构比对函数<br>
编译<br>
g++ main.cpp -o main<br>
运行<br>
./main map1 map2<br>
map1和map2为SCOP蛋白质数据文件处理后的数据文件<br>
## 数据处理程序
pdb2map.cpp为将SCOP蛋白质数据文件处理为map的程序<br>
编译<br>
g++ pdb2map.cpp -o pdb2map<br>
运行<br>
./pdb2map name.ent name.map<br>
