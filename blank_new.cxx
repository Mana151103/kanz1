#include"readData.h"

//コンパイルコマンドは
// g++ blank.cxx vector.o vectorArray.o readData.cxx

void computeDistances(const Vector& testData, const VectorArray& trainData, Vector& distance, Vector& index);

void selectTopK(Vector& distance, Vector& index, int k);

Vector countLabels(const Vector& trainCorrect, const Vector& index, int k, int numLabels);

int getMajorityLabel(const Vector& labelCount);

void printResult(VectorArray& tmp_testD, VectorArray& trainD, Vector& minIndexArray, Vector& minDistArray, Vector& testLabel, int class0Num, int class1Num, int nonClassNum);

double correlationCoefficient(VectorArray testData, Vector testLabel, VectorArray trainData, Vector trainCorrect, int c);

int main(void){
	/*------ここからデータの読み取り------*/
	VectorArray trainData;
	Vector trainCorrect = read_data("2d2c-train.plot", trainData);
	VectorArray tmp_testData;
	Vector testCorrect = read_dataWithoutLabel("2d2c-test.data", tmp_testData);
	/*------ここまでデータの読み取り------*/

	//追加：kの定義
	int k = Hyperparameter;

	//(読み込んだデータの画面表示)
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

	//テストデータの識別結果を保存するための配列
	Vector testLabel(tmp_testData.rows());
	//追加：最小距離保存のための配列
	Vector minDistArray(tmp_testData.rows());
	//追加：最小距離対象トレーニングデータ番号保存のための配列
	Vector minIndexArray(tmp_testData.rows());

	for (int i=0; i<tmp_testData.rows(); i++){
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
		int bestLabel = getMajorityLabel(labelCount);

		//変更：テストデータのラベル確定
		testLabel[i] = bestLabel;
	}

	//追加：各ラベルのデータ数
	int class0Number = 0, class1Number = 0, nonClassNumber = 0;

	//追加：結果を最後にまとめて表示
	printResult(tmp_testData, trainData, minIndexArray, minDistArray, testLabel, class0Number, class1Number, nonClassNumber);

	//追加：各ラベルの個数表示
	std::cout << std::endl;
	std::cout << "----- number of data -----" << std::endl;
	std::cout << "Class.0 data: " << class0Number << std::endl;
	std::cout << "Class.1 data: " << class1Number << std::endl;
	std::cout << "Unclassified data: " << nonClassNumber << std::endl;

	//相関係数の表示
	std::cout << std::endl;
	std::cout << "----- Correlation coefficient -----" << std::endl;
	std::cout << "Class.0: r=" << correlationCoefficient(tmp_testData, testLabel, trainData, trainCorrect, 0) << std::endl;
	std::cout << "Class.1: r=" << correlationCoefficient(tmp_testData, testLabel, trainData, trainCorrect, 1) << std::endl;
	std::cout << "All data: r=" << correlationCoefficient(tmp_testData, testLabel, trainData, trainCorrect, -1) << std::endl;

	//記録保存
	recordClassified(tmp_testData, testLabel);

	return 0;
}

//相関係数計算
double correlationCoefficient(VectorArray testData, Vector testLabel, VectorArray trainData, Vector trainCorrect, int c){
	double r, Sxy, Sxx, Syy, xyAve = 0, xAve = 0, x2Ave = 0, yAve = 0, y2Ave = 0;
	int d = 0;

	//Sxy = xyAve - xAve*yAve
	//Sxx = x2Ave - (xAve)^2
	//Syy = y2Ave - (yAve)^2
	//r = Sxy / sqrt(Sxx*Syy)

	//↓変数の説明↓
	//xAve: xの平均
	//x2Ave: xの2乗の平均
	//yAve: yの平均
	//y2Ave: yの2乗の平均
	//xyAve: xyの平均
	//Sxx: xの分散
	//Syy: yの分散
	//Sxy: x,yの共分散
	//r: 相関係数
	//d: 該当するデータの数

	//↓引数の説明↓
	//testData: テストデータを格納しているVectorArray
	//testLabel: テストデータのクラスを格納しているVector
	//trainData: 教師データ(トレインデータ)を格納しているVectorArray
	//trainCorrect: 教師データ(トレインデータ)のクラスを格納しているVector
	//c: int型の参照するクラスのデータのクラス識別番号(c=-1のときに全データを参照する)

	for(int i=0;i<testData.rows();i++){
		if(testLabel[i]==c || c==-1){
		xAve += testData[i][0];
		x2Ave += testData[i][0] * testData[i][0];
		yAve += testData[i][1];
		y2Ave += testData[i][1] * testData[i][1];
		xyAve += testData[i][0] * testData[i][1];
		d++;
		}
	}
	for(int i=0;i<trainData.rows();i++){
		if(trainCorrect[i]==c || c==-1){
		xAve += trainData[i][0];
		x2Ave += trainData[i][0] * trainData[i][0];
		yAve += trainData[i][1];
		y2Ave += trainData[i][1] * trainData[i][1];
		xyAve += trainData[i][0] * trainData[i][1];
		d++;
		}
	}
	if(d!=0){
		xAve /= double(d);
		x2Ave /= double(d);
		yAve /= double(d);
		y2Ave /= double(d);
		xyAve /= double(d);
		Sxy = xyAve - xAve*yAve;
		Sxx= x2Ave - xAve*xAve;
		Syy= y2Ave - yAve*yAve;
		r = Sxy / sqrt(Sxx*Syy);
	}
	else{
		r=0;
	}

	return r;
}

void computeDistances(const Vector& testData, const VectorArray& trainData, Vector& distance, Vector& index){
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

Vector countLabels(const Vector& trainCorrect, const Vector& index, int k, int numLabels){
	Vector labelCount(numLabels);
    for (int m = 0; m < k; ++m) {
        int trainIndex = static_cast<int>(index[m]);
        int label = static_cast<int>(trainCorrect[trainIndex]);
        labelCount[label]++;
    }
    return labelCount;
}

int getMajorityLabel(const Vector& labelCount){
	int bestLabel = 0, maxCount = 0;
	for (int n=0; n<labelCount.size(); n++){
		if (labelCount[n] > maxCount){
			maxCount = labelCount[n];
			bestLabel = n;
		}
	}
	return bestLabel;
}

void printResult(VectorArray& tmp_testD, VectorArray& trainD, Vector& minIndexArray, Vector& minDistArray, Vector& testLabel, int class0Num, int class1Num, int nonClassNum){
	//追加：結果を最後にまとめて表示
	std::cout << "----- Result -----" << std::endl;

	for (int i=0; i<tmp_testD.rows(); i++){
		std::cout << "TestData." << i << ": [" << tmp_testD[i][0] << tmp_testD[i][1] << "]" << std::endl;
		std::cout << "minIndex[" << i << "] :" << minIndexArray[i]
				<< " -> [" << trainD[minIndexArray[i]][0]
				<< ", " << trainD[minIndexArray[i]][1] << "]"
				<< ", minDistance[" << i << "] :" << minDistArray[i] << ", ";
		std::cout << "Class -> " << testLabel[i] << std::endl;
		if (testLabel[i] == 0){
			class0Num++;
		}
		else if (testLabel[i] == 1){
			class1Num++;
		}
		else{ //分類不可の時（0,1ラベルの個数が同じ）
			nonClassNum++;
		}
	}
}
