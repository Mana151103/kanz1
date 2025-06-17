#include"readData.h"

//コンパイルコマンドは
// g++ blank.cxx vector.o vectorArray.o readData.cxx

int main(void){
	/*------ここからデータの読み取り------*/
	VectorArray trainData;
	Vector trainCorrect = read_data("2d2c-train.plot", trainData);
	VectorArray tmp_testData;
	Vector testCorrect = read_dataWithoutLabel("2d2c-test.data", tmp_testData);
	/*------ここまでデータの読み取り------*/

	//追加：kの定義
	int k = Hyperparameter;
<<<<<<< HEAD
=======

>>>>>>> 489eeddc2376a702ec3208d8122e4f01569baa6b
	//(読み込んだデータの画面表示)
	// std::cout << "Training-Data:" << std::endl;
	// for(int x=0; x<trainData.rows(); x++){
	// 	std::cout << "Train." << x << ": "
	// 		<< "[" << trainData[x][0] << ", " << trainData[x][1] << "]\n";
	// }
	// std::cout << std::endl;

	// std::cout << "Test-Data:" << std::endl;
	// for(int y=0; y<tmp_testData.rows(); y++){
	// 	std::cout << "Test." << y << ": "
	// 		<< "[" << tmp_testData[y][0] << ", " << tmp_testData[y][1] << "]\n";
	// }
	// std::cout << std::endl;

	//テストデータの識別結果を保存するための配列
	Vector testLabel(tmp_testData.rows());

	for(int i=0; i<tmp_testData.rows(); i++){

		//注目しているテストデータを格納するためのインスタンス
		Vector testData = tmp_testData[i];

<<<<<<< HEAD
		//追加：テストデータとトレインデータの距離、トレーニングデータ番号を格納するベクトル
		//std::vector<std::pair<double, int>> distIndexPairs;

=======
>>>>>>> 489eeddc2376a702ec3208d8122e4f01569baa6b
		//各トレーニングデータとの距離を格納するための配列
		Vector distance(trainData.rows());
		//追加：各トレーニングデータ番号を格納するための配列
		Vector index(trainData.rows());
		
		//各トレーニングデータとの距離を計算
		for (int j=0; j<trainData.rows(); j++){
			double dx = testData[0] - trainData[j][0];
			double dy = testData[1] - trainData[j][1];
			double dist = dx*dx + dy*dy;
<<<<<<< HEAD
			//distIndexPairs.push_back({distance, j});
=======
>>>>>>> 489eeddc2376a702ec3208d8122e4f01569baa6b
			distance[j] = dist;
			index[j] = j;
		}

		//追加：ソートしてベクトルdistanceとindexの中身を距離の短い順に並び変え
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

		//追加：上位k個内の各ラベルの個数カウント
<<<<<<< HEAD
		//std::map<int, int> labelCount;//ラベルごとの出現数を記録
		Vector labelCount(2);
		// for(int m=0; m<k; m++){
		// 	int trainIndex = index[m];
		// 	int label = trainCorrect[trainIndex][2];
		// 	labelCount[label]++;
		// }
=======
		Vector labelCount(2);
>>>>>>> 489eeddc2376a702ec3208d8122e4f01569baa6b
		for (int m=0; m<k; m++){
			int trainIndex = index[m];
			int label = static_cast<int>(trainCorrect[trainIndex]);
			labelCount[label]++;
		}

		//追加：最も多いラベルを多数決で決定する
		int bestLabel = 0, maxCount = 0;
		for (int n=0; n<labelCount.size(); n++){
			if (labelCount[n] > maxCount){
				maxCount = labelCount[n];
				bestLabel = n;
			}
		}
<<<<<<< HEAD
	// 	for (const auto& entry : labelCount) {
	// 	if (entry.second > maxCount) {
	// 		maxCount = entry.second;
	// 		bestLabel = entry.first; // ←こっちが正しい
	// 	}
	// }
=======
>>>>>>> 489eeddc2376a702ec3208d8122e4f01569baa6b

		//変更：テストデータのラベル確定
		testLabel[i] = bestLabel;
		//追加：スコア（確信度）計算と画面表示
		double confidence = static_cast<double>(maxCount) / k;
		std::cout << "Test." << i
		<< ": Predicted Label = " << bestLabel
			  << ", Confidence = " << confidence * 100 << "%\n";
<<<<<<< HEAD
		// //最小の非類似度の値を保持するための変数
		// double minDist = pow(10,300); //10の300乗
		
		// //最小の非類似度を示すトレーニングデータが何番目なのかを記録するための変数
		// int minIndex;
		
		// //最近傍のトレーニングデータを探索
		// for(int j=0; j<trainData.rows(); j++){
		// if(minDist > distance[j])
		// {
		// minDist = distance[j];
		// minIndex = j;
		// }
		// }

		// //問1確認用
		// for(int x=0; x<trainData.rows(); x++){
		// std::cout << "Train." << x << ": "
		// 			<< "[" << trainData[x][0] << ", " << trainData[x][1] << "]" << "\t";
		// std::cout << "Distance:" << distance[x] << std::endl;
		// }
		// std::cout << std::endl;

		// //問2確認用
		// std::cout << "minIndex :" << minIndex << std::endl << std::endl;
=======
>>>>>>> 489eeddc2376a702ec3208d8122e4f01569baa6b
	}
	
	//記録保存
	recordClassified(tmp_testData, testLabel);

	return 0;
}
