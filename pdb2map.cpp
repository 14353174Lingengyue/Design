#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

class Atom {
	public:
		double x;
		double y;
		double z;
		Atom(double x, double y, double z){
			this->x = x;
			this->y = y;
			this->z = z;
		}
		double getDistance(Atom atom){
			return sqrt(pow(x - atom.x, 2) + pow(y - atom.y, 2) + pow(z - atom.z, 2));
		}
};

int main(int argc, char* argv[]){
	string infilename = argv[1];
	string outfilename = argv[2];
	ifstream infile;
	ofstream outfile;
	double d0 = 4;
	infile.open(infilename);
	if (!infile){
		cout << "File no exist!" << endl;
		return 0;
	}
	string line;
	string label;
	string fold;
	vector<Atom> atoms;
	while (getline(infile, line)){
		istringstream is(line);
		is >> label;
		if (label == "REMARK"){
			string s1, s2, s3, s4;
			is >> s1 >> s2 >> s3 >> s4;
			if (s3 == "SCOP-sccs:"){
				fold = "";
				int l = s4.length();
				int ct = 0;
				for (int i = 0; i < l; i++){
					if (s4[i] == '.'){
						ct++;
						if (ct == 2){
							break;
						}
					}
					fold += s4[i];
				}
			}
		}
		if (label == "ATOM"){
			int atomi, acid;
			string atype;
			is >> atomi >> atype;
			if (atype != "CA"){
				continue;
			}
			string s1, s2;
			double x, y, z;
			is >> s1 >> s2 >> acid >> x >> y >> z;
			//cout << x << " " << y << " " << z << endl;
			Atom a(x, y, z);
			atoms.push_back(a);
		}
		
	}
	int len = atoms.size();
	outfile.open(outfilename);
	outfile << "FOLD" << " " << fold << endl;
	outfile << "LEN" << " " <<  len << endl;
	for (int i = 0; i < len; i++){
		for (int j = i + 1; j < len; j++){
			double dist = atoms[i].getDistance(atoms[j]);
			if (dist < 2 * d0){
				outfile << i << " " << j << " ";
				//outfile << atoms[j].x - atoms[i].x << " ";
				//outfile << atoms[j].y - atoms[i].y << " ";
				//outfile << atoms[j].z - atoms[i].z << " ";
				outfile << dist << endl;
			}
		}
	}
	/*for (int i = 0; i < len; i++){
		for (int j = 0; j <= i; j++){
			outfile << atoms[i].getDistance(atoms[j]);
			if (i != j){
				outfile << " ";
			}
		}
		outfile << endl;
	}*/
	infile.close();
	outfile.close();
}
