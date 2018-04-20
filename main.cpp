#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

double c1 = 1.6;
double c2 = 2;
double c3 = 10;
double alpha = 1;

int readData(ifstream& file, int& len, string& fold, vector<vector<double> >& dist);

void init_SCO(vector<vector<double> >& SCO, vector<vector<double> > dist1, vector<vector<double> > dist2);

double get_SCO(vector<double> v1, vector<double> v2);

double Gauss(double x, double c){
	return exp(-x * x /(2 * c * c));
}

double distSCO(double dist, double c){
	return exp(-abs(dist)/(2 * c * c));
}

int main(int argc, char* argv[]){
	if (argc < 3){
		cout << "Argument Error! Please input two filename" << endl;
		return 0;
	}
	char fn1[20];
	char fn2[20];
	strcpy(fn1, argv[1]);
	strcpy(fn2, argv[2]);
	for (int i = 3; i < argc - 1; i++){
		if (strcmp(argv[i], "-c1") == 0){
			c1 = atof(argv[i + 1]);
		}
		if (strcmp(argv[i], "-c2") == 0){
			c2 = atof(argv[i + 1]);
		}
		if (strcmp(argv[i], "-c3") == 0){
			c3 = atof(argv[i + 1]);
		}
		if (strcmp(argv[i], "-alpha") == 0){
			alpha = atof(argv[i + 1]);
		}
	}
	ifstream file1, file2;
	file1.open(fn1);
	file2.open(fn2);
	if (!file1 || !file2){
		cout << "File not exist!" << endl;
		return 0;
	}
	int len1, len2;
	string fold1, fold2;
	vector<vector<double> > vec1, vec2;
	if (!readData(file1, len1, fold1, vec1)){
		cout << fn1 << " data not valid" << endl;
		return 0;
	}
	if (!readData(file2, len2, fold2, vec2)){
		cout << fn2 << " data not valid" << endl;
		return 0;
	}
	vector<vector<double> > SCO(len1 + 1, vector<double>(len2 + 1, 0));
	init_SCO(SCO, vec1, vec2);
	double maxSCO = 0;
	int sepX[len1 + 1][len2 + 1];
	int sepY[len1 + 1][len2 + 1];
	int sepx, sepy;
	for (int i = 0; i < len1 + 1; i++){
		for (int j = 0; j < len2 + 1; j++){
			if (i == 0 || j == 0){
				sepX[i][j] = 0;
				sepY[i][j] = 0;
				continue;
			}
			double newSCO = SCO[i - 1][j - 1] + SCO[i][j];
			sepX[i][j] = sepX[i - 1][j - 1];
			sepY[i][j] = sepY[i - 1][j - 1];
			double co = len1 / 6.0;
			co = min(10.0, c3);
			for (int k = 1; k < co && i - k > 0; k++){
				double tmp = SCO[i - k][j] * Gauss(k, c3);
				if (tmp > newSCO){
					newSCO = tmp;
					sepX[i][j] = sepX[i - k][j]; 
					sepY[i][j] = sepY[i - k][j] + k;
				}
			} 
			co = len2 / 6.0;
			co = min(10.0, c3);
			for (int k = 1; k < co && j - k > 0; k++){
				double tmp = SCO[i][j - k] * Gauss(k, c3);
				if (tmp > newSCO){
					newSCO = tmp;
					sepX[i][j] = sepX[i][j - k] + k;
					sepY[i][j] = sepY[i][j - k];
				}
			}
			SCO[i][j] = newSCO;
			if (maxSCO < newSCO){
				maxSCO = newSCO;
				sepx = sepX[i][j];
				sepy = sepY[i][j];
			}
		}
	}
	int L;
	if (len1 < len2){
		L = len1 - sepy;
	}
	else{
		L = len2 - sepx;
	}
	maxSCO = maxSCO / pow(L, alpha);
	int sameFold = 0;
	if (fold1 == fold2){
		sameFold = 1;
	}
	cout << maxSCO << " " << sameFold << endl;
} 

int readData(ifstream& file, int& len, string& fold, vector<vector<double> >& dist){
	string line;
	if (getline(file, line)){
		istringstream iss(line);
		string label;
		iss >> label;
		if (label == "FOLD"){
			iss >> fold;
		}
	}
	if (getline(file, line)){
		istringstream iss(line);
		string label;
		iss >> label;
		if (label == "LEN"){
			iss >> len;
		}
	}
	dist.resize(len);
	while(getline(file, line)){
		istringstream iss(line);
		int a, b;
		double d;
		iss >> a >> b >> d;
		if (a >= len || a < 0 || b >= len || b < 0){
			return 0;
		}
		dist[a].push_back(d);
		dist[b].push_back(d);
	}
	return len;
}

void init_SCO(vector<vector<double> >& SCO, vector<vector<double> > dist1, vector<vector<double> > dist2){
	int len1 = dist1.size();
	int len2 = dist2.size();
	for(int i = 0; i < len1; i++){
		int n = dist1[i].size();
		for(int j = 0; j < len2; j++){
			int m = dist2[j].size();
			if (n == 0 || m == 0){
				SCO[i + 1][j + 1] = 0;
				continue;
			}
			SCO[i + 1][j + 1] = get_SCO(dist1[i], dist2[j]);
		}
	}
}

double get_SCO(vector<double> v1, vector<double> v2){
	int row = v1.size();
	int col = v2.size();
	double maxSCO = 0;
	double M[row + 1][col + 1];
	int sepX[row + 1][col + 1];
	int sepY[row + 1][col + 1];
	int sepx, sepy;
	for (int i = 0; i < row + 1; i++){
		for (int j = 0; j < col + 1; j++){
			if (i == 0 || j == 0){
				M[i][j] = 0;
				sepX[i][j] = sepY[i][j] = 0;
				continue;
			}
			double dist = v1[i - 1] - v2[j - 1];
			double SCO = M[i - 1][j - 1] + distSCO(dist, c1);
			sepX[i][j] = sepX[i - 1][j - 1];
			sepY[i][j] = sepY[i - 1][j - 1];
			for (int k = 1; k < 5 && i - k > 0; k++){
				double tmp = M[i - k][j] * Gauss(k, c2);
				if (SCO < tmp){
					SCO = tmp;
					sepX[i][j] = sepX[i - k][j];
					sepY[i][j] = sepY[i - k][j] + k;
				}
			}
			for (int k = 1; k < 5 && j - k > 0; k++){
				double tmp = M[i][j - k] * Gauss(k, c2);
				if (SCO < tmp){
					SCO = tmp;
					sepX[i][j] = sepX[i][j - k] + k;
					sepY[i][j] = sepY[i][j - k];
				}
			}
			M[i][j] = SCO;
			if (maxSCO < SCO){
				maxSCO = SCO;
				sepx = sepX[i][j];
				sepy = sepY[i][j];
			}
		}
	}
	int L;
	if (row < col){
		L = row - sepy;
	}
	else{
		L = col - sepx;
	}
	maxSCO = maxSCO / pow(L, alpha);
	return maxSCO;
}
