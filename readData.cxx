#include"readData.h"

//std::stringは使用禁止，時間がないから後回しにしてるだけ
/*
  [input]
  file:データが記述されているファイル．
  [output]
  data:データ数を行，次元数を列に持つ，データが格納された行列．
 */
VectorArray readTestData(const std::string &file, int dim){
  std::ifstream ifs(file);
  if(!ifs){
    std::cerr << "ファイル: " << file << " を開くことができませんでした．" << std::endl;
    exit(1);
  }
  
  std::vector<std::vector<double>> tmpData;
  double tmpValue;
  while(ifs >> tmpValue){
    std::vector<double> arg(dim);
    arg[0] = tmpValue;
    for(int i=1; i<dim; i++){
      ifs >> arg[i];
    }
    tmpData.emplace_back(arg);
  }
  ifs.close();
  
  VectorArray data(tmpData.size(), dim);
  for(int i=0; i<data.rows(); i++){
    for(int j=0; j<dim; j++){
      data[i][j] = tmpData[i][j];
    }
  }
  
  return data;
}

/*
  [input]
    file:正解ラベルが記述されているファイル
  [output]
    label:各データが属するクラスインデックスを保有する配列
 */
Vector readTestCorrect(const std::string &file){
  std::ifstream ifs(file);
  if(!ifs){
    std::cerr << "ファイル: " << file << " を開くことができませんでした．" << std::endl;
    exit(1);
  }
  
  std::vector<int> tmpLabel;
  int tmpValue;
  while(ifs >> tmpValue){
    tmpLabel.emplace_back(tmpValue);
  }
  ifs.close();

  Vector label(tmpLabel.size());
  for(int i=0; i<label.size(); i++){
    label[i] = tmpLabel[i];
  }
  return label;
}

/*
  識別結果を，要素+ラベルの形でファイルに出力するための関数
  [input]
  file: テストデータの要素
  label: テストデータの識別結果，或いは正解
 */
void recordClassified(const VectorArray &data, const Vector &label){
  std::string result_file("classified.plot");
  std::ofstream ofs(result_file);
  if(!ofs){
    std::cerr << "ファイル: " << result_file << " を開くことができませんでした．" << std::endl;
    exit(1);
  }

  for(int i=0; i<data.rows(); i++){
    ofs << data[i][0] << "\t" << data[i][1] << "\t" << label[i] << std::endl;
  }
  ofs.close();

  std::cout << "識別結果を " << result_file << " に出力しました．" << std::endl;
  return;
}

/*
  次元数の指定なしでプロット用ファイルから情報を読み込むための関数．
  長所として，次元数，個体数の事前情報なしで読み取りが可能．
  また，ファイルの各行の要素数に過不足がないことも前提とする．(例えば，次元数2であれば，各行の要素数は(次元数+ラベルの)2+1=3になる．)
  [input]
    fileName: 要素の記述されたファイルの名前．
    data: データ要素を入力する対象．ポインタで直接入力する．
  [output]
    引数で指定されたdataと戻り値(resultLabel)にデータの要素，クラスラベルが入力される．
*/
Vector read_data(const std::string &fileName, VectorArray &data){
  //要素間の区切り文字の指定
  char key = '\t';

  //個体数，次元数取得のための作業
  std::ifstream ifs_getSize(fileName);
  if(!ifs_getSize){
    std::cerr << "Ifs:" << fileName << " can't open!" << std::endl;
    exit(1);
  }
//次元数取得
  int dim = 0;
  std::string str;
  getline(ifs_getSize, str);
  std::istringstream iss(str);
  std::string token;
  while (iss >> token) {
    dim++;
  }
  dim--;

  //個体数取得
  int num = 1;
  while(getline(ifs_getSize, str)){
    num++;
  }
  ifs_getSize.close();
  
  //要素の読み取り
  std::ifstream ifs_readData(fileName);
  if(!ifs_readData){
    std::cerr << "Ifs:" << fileName << " can't open!" << std::endl;
    exit(1);
  }
  
  VectorArray resultData(num, dim);
  Vector resultLabel(num);
  for(int i=0; i<num; i++){
    for(int j=0; j<dim; j++){
      ifs_readData >> resultData[i][j];
    }
    ifs_readData >> resultLabel[i];
  }
  ifs_readData.close();

  data = resultData;
  return resultLabel;
}

Vector read_dataWithoutLabel(const std::string &fileName, VectorArray &data){
  //要素間の区切り文字の指定
  char key = '\t';

  //個体数，次元数取得のための作業
  std::ifstream ifs_getSize(fileName);
  if(!ifs_getSize){
    std::cerr << "Ifs:" << fileName << " can't open!" << std::endl;
    exit(1);
  }

//次元数取得
  int dim = 0;
  std::string str;
  getline(ifs_getSize, str);
  std::istringstream iss(str);
  std::string token;
  while (iss >> token) {
    dim++;
  }

  //dim--;元々ラベル用列の分を差し引いていた箇所を無効化

  //個体数取得
  int num = 1;
  while(getline(ifs_getSize, str)){
    num++;
  }
  ifs_getSize.close();
  
  //要素の読み取り
  std::ifstream ifs_readData(fileName);
  if(!ifs_readData){
    std::cerr << "Ifs:" << fileName << " can't open!" << std::endl;
    exit(1);
  }
  
  VectorArray resultData(num, dim);
  Vector resultLabel(num);
  for(int i=0; i<num; i++){
    for(int j=0; j<dim; j++){
      ifs_readData >> resultData[i][j];
    }
  }
  ifs_readData.close();

  data = resultData;
  return resultLabel;
}




/*
  次元数の指定のみでプロット用ファイルから情報を読み込むための関数．
  一度std::vectorを経由しなければならない冗長性が欠点．
  [input]
    fileName: 要素の記述されたファイルの名前．
    data: データ要素を入力する対象．ポインタで直接入力する．
    label: ラベルを入力する対象．ポインタで直接入力する．
    dimension: 扱うデータの次元数．
  [output]
    引数で指定されたdataとlabelにデータの要素，クラスラベルが入力される．
*/
void read_data2(const std::string &fileName,
		VectorArray &data, Vector &label, int dimension){
  std::vector<std::vector<double>> dataList;
  std::vector<int> labelList;

  std::ifstream ifs(fileName);
  if(!ifs){
    std::cerr << "Ifs:" << fileName << " can't open!" << std::endl;
    exit(1);
  }
  
  double tmpValue;
  while(ifs >> tmpValue){
    std::vector<double> tmpObject(dimension);
    for(int i=0; i<dimension; i++){
      tmpObject[i] = tmpValue;
      ifs >> tmpValue;
    }
    dataList.emplace_back(tmpObject);
    labelList.emplace_back((int)tmpValue);
  }
  ifs.close();

  VectorArray resultData(dataList.size(), dimension);
  Vector resultLabel(labelList.size());
  for(int i=0; i<dataList.size(); i++){
    for(int j=0; j<dimension; j++){
      resultData[i][j] = dataList[i][j];
    }
    resultLabel[i] = labelList[i];
  }

  data = resultData;
  label = resultLabel;

  return;
}

/*
  次元数の指定なしでプロット用ファイルから情報を読み込むための関数．
  1つ目の関数と異なり，dataとlabelの配列長が設定されていなければならない．
  (空のインスタンスを引数に渡すとエラーになる可能性)
  [input]
    fileName: 要素の記述されたファイルの名前．
    data: データ要素を入力する対象．ポインタで直接入力する．
    label: ラベルを入力する対象．ポインタで直接入力する．
  [output]
    引数で指定されたdataとlabelにデータの要素，クラスラベルが入力される．
*/
void read_data3(const std::string &fileName,
		VectorArray &data, Vector &label){
  std::ifstream ifs(fileName);
  if(!ifs){
    std::cerr << "Ifs:" << fileName << " can't open!" << std::endl;
    exit(1);
  }

  for(int i=0; i<data.rows(); i++){
    for(int j=0; j<data.cols(); j++){
      ifs >> data[i][j];
    }
    ifs >> label[i];
  }
  ifs.close();
  
  return;
}
