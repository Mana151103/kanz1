#include "readData.h"

//コンパイルコマンドは
// g++ blank_new2.cxx blank_function.cxx vector.o vectorArray.o readData.cxx

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
	printData(trainData, tmp_testData);

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
		int maxCount = 0;
		int bestLabel = getMajorityLabel(labelCount, maxCount);

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
	std::cout << "Class.? data: " << nonClassNumber << std::endl;

	//相関係数の表示
	std::cout << std::endl;
	std::cout << "----- correlationCoefficient -----" << std::endl;
	std::cout << "Class.0: r=" << correlationCoefficient(tmp_testData, testLabel, trainData, trainCorrect, 0) << std::endl;
	std::cout << "Class.1: r=" << correlationCoefficient(tmp_testData, testLabel, trainData, trainCorrect, 1) << std::endl;

	//記録保存
	recordClassified(tmp_testData, testLabel);

	return 0;
}