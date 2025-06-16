#include"readData.h"

//コンパイルコマンドは
// g++ programing.cxx vector.o vectorArray.o readData.cxx

int main(void){
  /*------ここからデータの読み取り------*/
  VectorArray trainData;
  Vector trainCorrect = read_data("2d2c-train.plot", trainData);
  VectorArray tmp_testData;
  Vector testCorrect = read_dataWithoutLabel("2d2c-test.data", tmp_testData);
  /*------ここまでデータの読み取り------*/

  /* ↓改変箇所↓ */
  int kNumber=1;
  int minIndexArray[tmp_testData.rows()][kNumber];
  double minDistArray[tmp_testData.rows()][kNumber];
  /* ↑改変箇所↑ */

  //(読み込んだデータの画面表示)
  std::cout << "Training-Data:" << std::endl;
  for(int x=0; x<trainData.rows(); x++){
    std::cout << "Train." << x << ": "
	      << "[" << trainData[x][0] << ", " << trainData[x][1] << "]\n";
  }
  std::cout << std::endl;

  std::cout << "Test-Data:" << std::endl;
  for(int y=0; y<tmp_testData.rows(); y++){
    std::cout << "Test." << y << ": "
	      << "[" << tmp_testData[y][0] << ", " << tmp_testData[y][1] << "]\n";
  }
  std::cout << std::endl;

  //テストデータの識別結果を保存するための配列
  Vector testLabel(tmp_testData.rows());

  for(int i=0; i<tmp_testData.rows(); i++){
    /* ↓改変箇所↓ */
    std::cout << "----- TestData: " << i << " -----" << std::endl;
    /* ↑改変箇所↑ */
    
    //注目しているテストデータを格納するためのインスタンス
    Vector testData = tmp_testData[i];

    //各トレーニングデータとの距離を格納するための配列
    Vector distance(trainData.rows());
    
    //各トレーニングデータとの距離を計算
    for(int j=0; j<trainData.rows(); j++){
      
      distance[j] = (testData[0]-trainData[j][0])*(testData[0]-trainData[j][0]) + (testData[1]-trainData[j][1])*(testData[1]-trainData[j][1]);
      
    }
    
    //最小の非類似度の値を保持するための変数
    double minDist = {pow(10,300)}; //10の300乗
    
    //最小の非類似度を示すトレーニングデータが何番目なのかを記録するための変数
    int minIndex;
    
    /* ↓改変箇所↓ */
    //最近傍のトレーニングデータを探索
    for(int j=0; j<trainData.rows(); j++){
      if(j==0 || minDist > distance[j])
      {
	minDist = distance[j];
	minIndex = j;
      }
    }
    /* ↑改変箇所↑ */
    
    /* ↓改変箇所↓ */
    //テストデータのラベルを決定
    if()
    testLabel[i] = trainCorrect[minIndex];
    /* ↑改変箇所↑ */
    
    for(int x=0; x<trainData.rows(); x++){
      std::cout << "Train." << x << ": "
                << "[" << trainData[x][0] << ", " << trainData[x][1] << "]" << "\t";
      std::cout << "Distance:" << distance[x] << std::endl;
    }
    std::cout << std::endl;

    /* ↓改変箇所↓ */
    std::cout << "TestData." << i << ": [" << testData[0] << ", " << testData[1] << "]"
	      << ", minIndex :" << minIndex
	      << " -> [" << trainData[minIndex][0] << ", " << trainData[minIndex][1] << "]"
	      << ", minDistance :" << minDist << std::endl << std::endl;

    minIndexArray[i]=minIndex;
    minDistArray[i]=minDist;
    /* ↑改変箇所↑ */
    
  }

  /* ↓改変箇所↓ */
  int class0Number=0;
  int class1Number=0;
  int nonClassNumber=0;

  std::cout << "----- Result -----" << std::endl;
  
  for(int i=0;i<tmp_testData.rows();i++){
    std::cout << "TestData." << i << ": [" << tmp_testData[i][0] << ", " << tmp_testData[i][1] << "]";
    for(int j=0;j<kNumber;j++){
      std::cout << ", minIndex[" << j << "] :" << minIndexArray[i]
	      << " -> [" << trainData[minIndexArray[i][j]][0]
	      << ", " << trainData[minIndexArray[i][j]][1] << "]"
	      << ", minDistance[" << i <<  << minDistArray[i][j] << ", ";   
    }
    std::cout << "Class -> " << testLabel[i] << std::endl;
    if(testLabel[i]==0){
      class0Number++;
    }
    else if(testLabel[i]==1){
      class1Number++;
    }
    else{
      nonClassNumber++;
    }
  }

  std::cout << "Class.0 data: " << class0Number << std::endl;
  std::cout << "Class.1 data: " << class1Number << std::endl;
  std::cout << "Class.? data: " << nonClassNumber << std::endl;

  /* ↑改変箇所↑ */
 
  //記録保存
  recordClassified(tmp_testData, testLabel);

  return 0;
}

