#include <assert.h>
#include <iostream>
#include "leveldb/db.h"

using namespace std;

int main(){
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options,"./testdb",&db);//打开一个数据库
    std::string key = "asurily";
    std::string value = "asurily@gmail.com";

    status = db->Put(leveldb::WriteOptions(), key, value);//添加
    assert(status.ok());

    status = db->Get(leveldb::ReadOptions(), key, &value);//获取
    assert(status.ok());
    std::cout<<value<<std::endl;

    std::string key2 = "yooliee";
    status = db->Put(leveldb::WriteOptions(), key, key2);//修改（就是重新赋值）
    assert(status.ok());
    status = db->Get(leveldb::ReadOptions(), key, &value);
    cout<<key<<"=="<<value<<endl;
    status = db->Delete(leveldb::WriteOptions(),key);//删除
    assert(status.ok());
    status = db->Get(leveldb::ReadOptions(), key2, &value);
    assert(status.ok());
    cout<<key2<<"=="<<value<<endl;

    status = db->Get(leveldb::ReadOptions(), key, &value);
    if(!status.ok()){
        std::cerr << key << ": "<<status.ToString()<<std::endl;
    }else{
        std::cout << key <<"=="<<value<<std::endl;
    }
    delete db; //关闭数据库

    return 0;  
}
//git clone --depth 1 --recurse-submodules https://github.com/google/leveldb.git
//cd leveldb 
//mkdir -p build
//cd build
//cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF .
//make
//touch test.cc
//g++ -g -o leveldbit test.cc -I../include libleveldb.a -lpthread