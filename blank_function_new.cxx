#include "readData.h"

void printData(VectorArray& trainData, VectorArray& tmp_testData){
	std::cout << "Training-Data:" << std::endl;
	for (int x=0; x<trainData.rows(); x++){
		std::cout << "Train." << x << ": "
			<< "[" << trainData[x][0] << ", " << trainData[x][1] << "]\n";
	}
	std::cout << std::endl;

	std::cout << "Test-Data:" << std::endl;
	for (int y=0; y<tmp_testData.rows(); y++){
		std::cout << "Test." << y << ": "
			<< "[" << tmp_testData[y][0] << ", " << tmp_testData[y][1] << "]\n";
	}
	std::cout << std::endl;
}

void computeDistances(Vector& testData, VectorArray& trainData, Vector& distance, Vector& index){
    for (int j = 0; j < trainData.rows(); ++j) {
        double dx = testData[0] - trainData[j][0];
        double dy = testData[1] - trainData[j][1];
        distance[j] = dx * dx + dy * dy;
        index[j] = j;
    }
}

void selectTopK(Vector& distance, Vector& index, int k){
	for (int a=0; a<k; a++){
		int minIndex = a;
		for (int b=a+1; b<index.size(); b++){
			if (distance[b]< distance[minIndex]){
				minIndex = b;
			}
		}
		//a番目の要素とminIndexの要素を入れ替える
		std::swap(distance[a], distance[minIndex]);
		std::swap(index[a], index[minIndex]);
	}
}

Vector countLabels(Vector& trainCorrect, Vector& index, int k, int numLabels){
	Vector labelCount(numLabels);
    for (int m = 0; m < k; ++m) {
        int trainIndex = static_cast<int>(index[m]);
        int label = static_cast<int>(trainCorrect[trainIndex]);
        labelCount[label]++;
    }
    return labelCount;
}

int getMajorityLabel(Vector& labelCount, Vector& maxCount, int i){
	int bestLabel = 0;
	maxCount[i] = 0;

	for (int n=0; n<labelCount.size(); n++){
		if (labelCount[n] > maxCount[i]){
			maxCount[i] = labelCount[n];
			bestLabel = n;
		}
	}
	return bestLabel;
}

void serchK(VectorArray& tmp_testData, VectorArray& trainData, Vector& trainCorrect, Vector& testLabel, Vector& minDistArray, Vector& minIndexArray, int i,int k, Vector& maxCount){
	//注目しているテストデータを格納するためのインスタンス
	Vector testData = tmp_testData[i];

	//各トレーニングデータとの距離を格納するための配列
	Vector distance(trainData.rows());
	//追加：各トレーニングデータ番号を格納するための配列
	Vector index(trainData.rows());

	//各トレーニングデータとの距離を計算
	computeDistances(testData, trainData, distance, index);

	//追加：ソートしてベクトルdistanceとindexの中身を距離の短い順に並び変え
	selectTopK(distance, index, k);

	//追加：最小距離とそのトレーニング番号の保持
	minDistArray[i] = distance[0];
	minIndexArray[i] = index[0];

	//追加：上位k個内の各ラベルの個数カウント
	Vector labelCount = countLabels(trainCorrect, index, k, 2);

	//追加：最も多いラベルを多数決で決定する
	int bestLabel = getMajorityLabel(labelCount, maxCount, i);

	//変更：テストデータのラベル確定
	testLabel[i] = bestLabel;
}

double correlationCoefficient(VectorArray& testData, Vector& testLabel, VectorArray& trainData, Vector& trainCorrect, int c){
	double r, Sxy, Sxx, Syy, xyAve=0, xAve=0, x2Ave=0, yAve=0, y2Ave=0;
	int d=0;

	//Sxy=xyAve-xAve*yAve
	//Sxx=x2Ave-(xAve)^2
	//Syy=y2Ave-(yAve)^2
	//r=Sxy/sqrt(Sxx*Syy)

	for(int i=0;i<testData.rows();i++){
		if(testLabel[i]==c){
		xAve+=testData[i][0];
		x2Ave+=testData[i][0]*testData[i][0];
		yAve+=testData[i][1];
		y2Ave+=testData[i][1]*testData[i][1];
		xyAve+=testData[i][0]*testData[i][1];
		d++;
		}
	}
	for(int i=0;i<trainData.rows();i++){
		if(trainCorrect[i]==c){
		xAve+=trainData[i][0];
		x2Ave+=trainData[i][0]*trainData[i][0];
		yAve+=trainData[i][1];
		y2Ave+=trainData[i][1]*trainData[i][1];
		xyAve+=trainData[i][0]*trainData[i][1];
		d++;
		}
	}
	if(d!=0){
		xAve/=double(d);
		x2Ave/=double(d);
		yAve/=double(d);
		y2Ave/=double(d);
		xyAve/=double(d);
		Sxy=xyAve-xAve*yAve;
		Sxx=x2Ave-xAve*xAve;
		Syy=y2Ave-yAve*yAve;
		r=Sxy/sqrt(Sxx*Syy);
	}
	else{
		r=0;
	}

	return r;
}

void printResult(VectorArray& tmp_testD, VectorArray& trainD, Vector& trainCorrect, Vector& minIndexArray, Vector& minDistArray, Vector& testLabel, int k, Vector& maxCount){
	//追加：結果を最後にまとめて表示
	std::cout << "----- Result -----" << std::endl;
	//追加：各ラベルのデータ数
	int class0Number = 0, class1Number = 0, nonClassNumber = 0;

	for (int i=0; i<tmp_testD.rows(); i++){
		double confidence = static_cast<double>(maxCount[i]) / k;

		std::cout << "TestData." << i << ": [" << tmp_testD[i][0] << tmp_testD[i][1] << "]" << std::endl;
		std::cout << "minIndex[" << i << "] :" << minIndexArray[i]
				<< " -> [" << trainD[minIndexArray[i]][0]
				<< ", " << trainD[minIndexArray[i]][1] << "]"
				<< ", minDistance[" << i << "] :" << minDistArray[i] << ", ";
		std::cout << "Class -> " << testLabel[i] << ", Confidence["  << i << "] :" << confidence * 100 << "%" << std::endl;
		if (testLabel[i] == 0){
			class0Number++;
		}
		else if (testLabel[i] == 1){
			class1Number++;
		}
		else{ //分類不可の時（0,1ラベルの個数が同じ）
			nonClassNumber++;
		}
	}
	//追加：各ラベルの個数表示
	std::cout << std::endl;
	std::cout << "----- number of data -----" << std::endl;
	std::cout << "Class.0 data: " << class0Number << std::endl;
	std::cout << "Class.1 data: " << class1Number << std::endl;
	std::cout << "Class.? data: " << nonClassNumber << std::endl;

	//追加：相関係数の表示
	std::cout << std::endl;
	std::cout << "----- correlationCoefficient -----" << std::endl;
	std::cout << "Class.0: r=" << correlationCoefficient(tmp_testD, testLabel, trainD, trainCorrect, 0) << std::endl;
	std::cout << "Class.1: r=" << correlationCoefficient(tmp_testD, testLabel, trainD, trainCorrect, 1) << std::endl;
	std::cout << "All data: r=" << correlationCoefficient(tmp_testD, testLabel, trainD, trainCorrect, -1) << std::endl;
	std::cout << std::endl;
}