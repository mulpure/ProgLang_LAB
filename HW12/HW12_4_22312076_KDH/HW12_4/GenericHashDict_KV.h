
/*
솔루션명 : HW12_4_22312076_KDH
헤 더 명 : GenericHashDict_KV.h

프로그램의 목적 및 기능
    - GenericHashMap_KV를 상속받아 동일 키에 대한 다중 항목 저장 지원 (Dictionary/Multimap 동작)
    - insert()를 오버라이드하여 같은 키의 항목을 덮어쓰지 않고 버킷 끝에 추가
    - findAll()로 같은 키의 모든 항목을 [begin, end) 범위(Range)로 반환

클래스 목록
    - GenericHashDict_KV<K, V>
    - GenericHashDict_KV<K, V>::Range

특이사항
    - 같은 키를 가진 항목은 반드시 동일 버킷에 연속으로 저장됨 (해시 값이 같으므로)
    - findAll() 종료 후 p가 버킷 로컬 끝(itrEnd)이면 다음 비어있지 않은 버킷 첫 항목으로 보정
    - fgetVocaDict()는 파일 첫 줄을 해시 맵 이름으로 읽고, 이후 줄을 Voca로 파싱하여 삽입

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/06/05
*/
#ifndef HASH_DICTIONARY_H
#define HASH_DICTIONARY_H
#include "GenericHashMap_KV.h"
#define DEFAULT_HASH_SIZE 101

// GenericHashMap_KV를 상속하여 동일 키 다중 항목 저장을 지원하는 해시 딕셔너리
template <typename K, typename V>
class GenericHashDict_KV : public GenericHashMap_KV<K, V>
{
public:
    typedef typename GenericHashMap_KV<K, V>::Iterator Iterator;
    typedef typename GenericHashMap_KV<K, V>::BItor    BItor;
    int fgetVocaDict(std::istream& fin);

    // 같은 키를 가진 항목 범위([begin, end))를 표현하는 이터레이터 쌍 클래스
    class Range {
    private:
        Iterator _begin;
        Iterator _end;
    public:
        Range() = default;
        Range(const Iterator& b, const Iterator& e) : _begin(b), _end(e) {}
        Iterator begin() { return _begin; }
        Iterator end() { return _end; }
    };

public:
    GenericHashDict_KV(const std::string nm, int hashTblSize = DEFAULT_HASH_SIZE)
        : GenericHashMap_KV<K, V>(nm, hashTblSize) {}

    /*
    * 함수명 : insert (오버라이드)
    * 설  명 : HashMap과 달리 기존 값을 덮어쓰지 않고, 같은 키의 마지막 항목 뒤에 삽입
    *          (Dictionary/Multimap 동작: 동일 키 다중 항목 허용)
    */
    Iterator insert(const K& k, const V& v)
    {
        Iterator p = this->_find(k);
        BItor bkt = p.getBkt();
        Iterator itrEnd = Iterator(this->BA, bkt, bkt->end());

        //같은 키를 가진 기존 항목들을 모두 건너뛰어 삽입 위치 찾기
        while ((p != itrEnd) && ((*p).key() == k))
            GenericHashMap_KV<K, V>::_next(p);

        return this->_insert(p, Entry_KV<K, V>(k, v));
    }

    /*
    * 함수명 : findAll
    * 설  명 : 키 k와 일치하는 모든 항목을 Range로 반환
    *          Range의 [begin, end)로 for-loop 순회 가능
    * 반환값 : Range (키 k에 해당하는 첫 항목부터 마지막 항목 다음까지)
    */
    Range findAll(const K& k)
    {
        Iterator b = this->_find(k);
        //키를 찾지 못한 경우 빈 범위 반환
        if (GenericHashMap_KV<K, V>::endOfBkt(b))
            return Range(this->end(), this->end());

        Iterator p = b;
        BItor bkt = p.getBkt();
        Iterator itrEnd = Iterator(this->BA, bkt, bkt->end());

        //버킷 내에서 키 k인 항목을 모두 건너뛰어 범위 끝 계산
        while ((p != itrEnd) && ((*p).key() == k))
            GenericHashMap_KV<K, V>::_next(p);

        //p가 버킷 로컬 끝이면 전체 Iterator의 ++와 동일한 위치로 보정
        if (p == itrEnd) {
            ++bkt;
            while (bkt != this->BA.end() && bkt->empty()) ++bkt;
            p = (bkt == this->BA.end())
                ? this->end()
                : Iterator(this->BA, bkt, bkt->begin());
        }
        return Range(b, p);
    }
};

/*
* 함수명 : fgetVocaDict
* 설  명 : 파일 스트림에서 어휘 데이터를 읽어 해시 딕셔너리에 삽입
*          첫 줄은 DB 이름, 이후 각 줄은 Voca 항목으로 파싱
* 매개변수 : fin - 입력 파일 스트림
* 반환값 : int - 처리된 줄 수 (파싱 실패 포함)
*/
template <typename K, typename V>
int GenericHashDict_KV<K, V>::fgetVocaDict(std::istream& fin)
{
    std::string strLine;
    int num_vocas = 0;
    //첫 줄을 해시 맵 이름으로 저장
    fin >> this->name;
    getline(fin, strLine);  // 첫 줄 나머지 개행 문자 소비

    while (getline(fin, strLine)) {
        Voca voca;
        if (voca.fgetVoca(strLine))
            insert(voca.getKeyWord(), voca);  // Dictionary insert (다중 항목 허용)
        else
            std::cout << "Parsing failed: " << strLine << std::endl;
        num_vocas++;
    }
    return num_vocas;
}
#endif