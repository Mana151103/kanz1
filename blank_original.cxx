#include"readData.h"

//コンパイルコマンドは
// g++ blank.cxx vector.o vectorArray.o readData.cxx

int main(void){
  /*------ここからデータの読み取り------*/
  VectorArray trainData;
  Vector trainCorrect = read_data("2d2c-train.plot", trainData); //easy-train.plot → 2d2c-train.plot
  VectorArray tmp_testData;
  Vector testCorrect = read_dataWithoutLabel("2d2c-test.data", tmp_testData); //easy-test.data → 2d2c-test.data
  /*------ここまでデータの読み取り------*/
  

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

    //注目しているテストデータを格納するためのインスタンス
    Vector testData = tmp_testData[i];

    //各トレーニングデータとの距離を格納するための配列
    Vector distance(trainData.rows());
    
    //各トレーニングデータとの距離を計算
    for(int j=0; j<trainData.rows(); j++){
      /*----------(問1)に関する記述を行う----------*/
      distance[j] = (testData[0]-trainData[j][0])*(testData[0]-trainData[j][0]) + (testData[1]-trainData[j][1])*(testData[1]-trainData[j][1]);
      
    }
    
    //最小の非類似度の値を保持するための変数
    double minDist = pow(10,300); //10の300乗
    
    //最小の非類似度を示すトレーニングデータが何番目なのかを記録するための変数
    int minIndex;
    
    //最近傍のトレーニングデータを探索
    for(int j=0; j<trainData.rows(); j++){
      //if(/*----(問2)に関する記述----*/)
      if(j==0 || minDist > distance[j])
      {
	minDist = distance[j];
	minIndex = j;
      }
    }
    
    //テストデータのラベルを決定
    /*----------(問3)に関する記述を行う----------*/
    testLabel[i] = trainCorrect[minIndex];
    

    //問1確認用
    for(int x=0; x<trainData.rows(); x++){
      std::cout << "Train." << x << ": "
                << "[" << trainData[x][0] << ", " << trainData[x][1] << "]" << "\t";
      std::cout << "Distance:" << distance[x] << std::endl;
    }
    std::cout << std::endl;

    //問2確認用
    std::cout << "minIndex :" << minIndex << std::endl << std::endl;
    
  }
 
  //記録保存
  recordClassified(tmp_testData, testLabel);

  return 0;
}