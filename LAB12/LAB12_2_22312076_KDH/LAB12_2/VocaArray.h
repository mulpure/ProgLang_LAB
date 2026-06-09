/*
솔루션명 : Lab12_2_22312076_KDH
헤 더 명 : VocaArray.h

프로그램의 목적 및 기능
    - Voca 객체를 vector로 관리하는 배열 컨테이너 클래스 정의
    - 파일 스트림에서 어휘 데이터를 읽어 배열에 순차 저장

클래스 목록
    - VocaArray

특이사항
    - 헤더에 함수 정의 시 inline 키워드 필수 (ODR 위반 방지)
    - 첫 번째 줄을 DB 이름으로 읽은 후 나머지 줄을 Voca로 파싱
    - operator[]에서 범위 검사를 수행하여 out_of_range 예외 발생

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/06/05
*/
#ifndef VOCA_ARRAY_H
#define VOCA_ARRAY_H
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <regex>
#include "Voca.h"

class VocaArray
{
public:
    // 파일 스트림에서 DB 이름(첫 줄)과 어휘 데이터를 읽어 초기화
    VocaArray(std::ifstream& fin) {
        getline(fin, this->db_name);
        this->num_vocas = this->fgetVocaArray(fin);
    }
    VocaArray() { this->array_voca.clear(); this->num_vocas = 0; }
    ~VocaArray() { this->array_voca.clear(); }

    int    size() { return num_vocas; }
    int    getNumVocas() { return num_vocas; }
    Voca& operator[](int index);
    void   printVocaArray(std::ostream& fout);
    int    fgetVocaArray(std::istream& fin);

private:
    std::string db_name;
    int         num_vocas;
    std::vector<Voca> array_voca;
    bool _isValidIndex(int index);
};

// inline: 헤더에 함수 정의 시 다중 포함(ODR)으로 인한 링커 오류 방지
 bool VocaArray::_isValidIndex(int index)
{
    return (index >= 0 && index < num_vocas);
}

// 인덱스 유효성 검사 후 해당 Voca 참조 반환
Voca& VocaArray::operator[](int index)
{
    if (_isValidIndex(index))
        return this->array_voca[index];
    else
        throw std::out_of_range("VocaArray index out of range");
}

// DB 이름과 전체 항목 수, 각 Voca 내용을 순서대로 출력
inline void VocaArray::printVocaArray(std::ostream& fout)
{
    fout << this->db_name << "(num_vocas : " << this->num_vocas << ")" << std::endl;
    for (int i = 0; i < this->num_vocas; i++) {
        fout << this->array_voca[i];
        fout << std::endl;
    }
}

/*
* 함수명 : fgetVocaArray
* 설  명 : 입력 스트림에서 한 줄씩 읽어 Voca로 파싱 후 배열에 저장
* 매개변수 : fin - 입력 파일 스트림
* 반환값 : int - 파싱 성공한 Voca 개수
*/
int VocaArray::fgetVocaArray(std::istream& fin)
{
    std::string strLine;
    int num_vocas = 0;
    while (getline(fin, strLine)) {
        Voca voca;
        if (voca.fgetVoca(strLine)) {
            array_voca.push_back(voca);
            num_vocas++;
        }
        else {
            std::cout << "Parsing failed: " << strLine << std::endl;
        }
    }
    return num_vocas;
}
#endif