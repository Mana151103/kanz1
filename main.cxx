#include "readData.h"

//コンパイルコマンドは
// g++ main.cxx function.cxx vector.o vectorArray.o readData.cxx

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

	//追加：最小距離対象トレーニングデータ番号保存のための配列
	VectorArray minIndexArray(tmp_testData.rows(), k);
	//追加：最多ラベルの個数を保存するための配列
	Vector maxCount(tmp_testData.rows());

	//追加：k個探す
	for (int i=0; i<tmp_testData.rows(); i++){
		serchK(tmp_testData, trainData, trainCorrect, testLabel, minIndexArray, i, k, maxCount);
	}
	//追加：結果を最後にまとめて表示
	printResult(tmp_testData, trainData, trainCorrect, minIndexArray, testLabel, k, maxCount);

	//記録保存
	recordClassified(tmp_testData, testLabel);

	return 0;
}