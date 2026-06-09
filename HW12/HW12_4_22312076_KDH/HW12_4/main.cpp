/*
솔루션명 : HW12_4_22312076_KDH
소스코드 : main.cpp

프로그램의 목적 및 기능
    - 텍스트 파일에서 어휘(Voca) 데이터를 읽어 해시 딕셔너리에 저장
    - 해시 맵(GenericHashDict_KV)을 이용한 Key-Value 기반 어휘 관리
    - Iterator와 Range를 활용한 전체 순회 및 키워드 검색 수행
    - 버킷 크기 출력을 통한 해시 테이블 분포 확인

함수 목록
    - main

특이사항
    - GenericHashDict_KV 템플릿으로 string 키, Voca 값 쌍을 관리
    - findAll()을 통해 동일 키에 대한 다중 항목 검색 지원
    - Iterator/Range 패턴으로 해시 맵 전체 및 범위 순회 수행

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/06/05
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Voca.h"
#include "VocaArray.h"
#include "GenericHashDict_KV.h"
#include <string>
using namespace std;

#define HASH_TABLE_SIZE 101  // 해시 테이블 크기 (소수 사용으로 충돌 최소화)

/*
* 함수명 : main
* 설  명 : 어휘 데이터 파일을 읽어 해시 딕셔너리에 저장한 뒤,
*          전체 순회 출력 및 특정 키워드 검색 결과를 출력
* 매개변수 : 없음
* 반환값 : int (정상 종료 시 0)
*/
int main()
{
    //입력 파일 스트림 선언 및 어휘 데이터 파일 열기
    ifstream fin;
    fin.open("myVoca_data.txt");

    //파일 열기 실패 시 오류 메시지 출력 후 종료
    if (fin.fail()) {
        cout << "Fail to open input data file (myVoca_data.txt) !!" << endl;
        exit(-1);
    }

    //해시 딕셔너리 객체 생성 (키: string, 값: Voca, 크기: 101)
    GenericHashDict_KV<std::string, Voca>
        thesHashDict("My Thesaurus Hash Map", HASH_TABLE_SIZE);

    //해시 딕셔너리 포인터 및 Iterator/Range 변수 선언
    GenericHashDict_KV<std::string, Voca>* pHashDict = &thesHashDict;
    GenericHashDict_KV<std::string, Voca>::Iterator vocaHD_Iter;
    GenericHashDict_KV<std::string, Voca>::Range    vocaHD_Range;

    //파일에서 어휘 데이터를 읽어 해시 딕셔너리에 저장
    pHashDict->fgetVocaDict(fin);

    //해시 딕셔너리 전체 내용 출력 시작
    cout << "\n==============================================\n";
    cout << "Voca HashMap after fgetVocaDict() :" << endl;

    //순회에 사용할 보조 변수 선언
    string keyWord;
    Voca* pVoca, voca;
    Entry_KV<string, Voca> vocaEntry_KV;

    //Iterator를 이용해 해시 딕셔너리 처음부터 끝까지 순회
    vocaHD_Iter = pHashDict->begin();
    while (vocaHD_Iter != pHashDict->end())
    {
        //현재 항목에서 키(단어)와 값(Voca) 추출 후 출력
        vocaEntry_KV = *vocaHD_Iter;
        keyWord = vocaEntry_KV.getKey();
        voca = vocaEntry_KV.getValue();
        cout << keyWord << " : " << voca << endl;

        //다음 항목으로 이동
        ++vocaHD_Iter;
    }

    //해시 테이블의 버킷별 크기 분포 출력
    pHashDict->fprintBucketSizes(cout);

    //키워드 검색 테스트 시작 안내 출력
    cout << "\nTesting keyword findings in HashDict_Voca ...." << endl;

    //검색할 테스트 키워드 배열 정의
    std::string test_keywords[] = { "mean", "compromise", "duplicate" };

    //각 키워드에 대해 해시 딕셔너리에서 동의어(Voca) 검색 및 출력
    for (auto keyWord : test_keywords) {
        cout << "Thesaurus of " << keyWord << " in HashDict_Voca : \n";

        //findAll()로 해당 키워드의 모든 항목 범위(Range) 획득
        vocaHD_Range = pHashDict->findAll(keyWord);

        //Range 내 모든 항목을 순회하며 Voca 값 출력
        for (vocaHD_Iter = vocaHD_Range.begin();
            vocaHD_Iter != vocaHD_Range.end(); ++vocaHD_Iter) {
            voca = (*vocaHD_Iter).getValue();
            cout << voca << endl;
        }
    }

    return 0;
} 