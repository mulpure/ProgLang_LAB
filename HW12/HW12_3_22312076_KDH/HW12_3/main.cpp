/*
솔루션명 : HW12_3_22312076_KDH
소스코드 : main.cpp

프로그램의 목적 및 기능
    - 텍스트 파일에서 어휘(Voca) 데이터를 읽어 해시 맵에 저장
    - GenericHashMap_KV를 이용한 Key-Value 기반 어휘 관리
    - Iterator를 활용한 전체 순회 및 키워드 검색 수행
    - 버킷 크기 출력을 통한 해시 테이블 분포 확인

함수 목록
    - main

특이사항
    - GenericHashMap_KV 템플릿으로 string 키, Voca 값 쌍을 관리
    - find()를 이용한 단일 항목 검색 지원
    - Iterator를 이용한 해시 맵 전체 순회 수행

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 26/06/07
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "Voca.h"
#include "VocaArray.h"
#include "GenericHashMap_KV.h"

using namespace std;

#define HASH_TABLE_SIZE 101

/*
* 설  명 : 어휘 데이터 파일을 읽어 해시 맵에 저장한 뒤,
*          전체 순회 출력 및 특정 키워드 검색 결과를 출력
* 매개변수 : 없음
* 반환값 : int (정상 종료 시 0)
*/
int main()
{
    ifstream fin;
    fin.open("myVoca_data.txt");

    if (fin.fail())
    {
        cout << "Fail to open input data file (myVoca_data.txt) !!"
            << endl;
        exit(-1);
    }

    GenericHashMap_KV<string, Voca>
        vocaHashMap("My TOEIC Voca HashMap",
            HASH_TABLE_SIZE);

    GenericHashMap_KV<string, Voca>* pHashMap =
        &vocaHashMap;

    GenericHashMap_KV<string, Voca>::Iterator vocaHM_Iter;

    string strLine;
    Voca voca;

    // 첫 줄(DB 이름) 건너뛰기
    getline(fin, strLine);

    // 파일에서 어휘 데이터 읽어 HashMap에 저장
    while (getline(fin, strLine))
    {
        Voca voca;

        if (voca.fgetVoca(strLine))
        {
            pHashMap->insert(
                voca.getKeyWord(),
                voca);
        }
        else
        {
            cout << "Parsing failed : "
                << strLine
                << endl;
        }
    }

    cout << "\n==============================================\n";
    cout << "Voca HashMap after insertions of Voca :\n";

    string keyWord;
    Entry_KV<string, Voca> vocaEntry_KV;

    vocaHM_Iter = pHashMap->begin();

    while (vocaHM_Iter != pHashMap->end())
    {
        vocaEntry_KV = *vocaHM_Iter;

        keyWord = vocaEntry_KV.getKey();
        voca = vocaEntry_KV.getValue();

        cout << keyWord
            << " : "
            << voca
            << endl;

        ++vocaHM_Iter;
    }

    cout << endl;

    pHashMap->fprintBucketSizes(cout);

    cout << "\nTesting keyword findings in HashMap_Voca ...."
        << endl;

    string test_keywords[] =
    {
        "mean",
        "compromise",
        "duplicate"
    };

    for (auto keyWord : test_keywords)
    {
        cout << "\nSearching keyword : "
            << keyWord
            << endl;

        vocaHM_Iter = pHashMap->find(keyWord);

        if (vocaHM_Iter != pHashMap->end())
        {
            cout
                << (*vocaHM_Iter).getValue()
                << endl;
        }
        else
        {
            cout
                << "Not found !!"
                << endl;
        }
    }

    return 0;
}