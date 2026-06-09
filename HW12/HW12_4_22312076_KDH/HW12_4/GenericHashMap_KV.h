/*
솔루션명 : HW12_4_22312076_KDH
헤 더 명 : GenericHashMap_KV.h

프로그램의 목적 및 기능
    - 버킷 배열(vector<list<Entry_KV>>)로 구현한 제네릭 해시 맵 클래스 정의
    - Separate Chaining 방식으로 해시 충돌 해결
    - 동일 키 삽입 시 기존 값을 덮어씀 (Map 동작)

클래스 목록
    - GenericHashMap_KV<K, V>
    - GenericHashMap_KV<K, V>::Iterator

특이사항
    - 해시 함수: std::hash<K>{}(k) % 버킷 수 (소수 101 권장)
    - Iterator는 (버킷 배열 포인터, 버킷 이터레이터, 항목 이터레이터) 3개 필드로 구성
    - friend class Iterator 선언으로 Iterator가 private 필드에 직접 접근 가능

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/06/05
*/
#ifndef GenericHashMap_KV_H
#define GenericHashMap_KV_H
#include <vector>
#include <list>
#include <functional>
#include <iostream>
#include <iomanip>
#include "Entry_KV.h"

template <typename K, typename V>
class GenericHashMap_KV {
protected:
    // 버킷: Entry_KV를 저장하는 연결 리스트
    typedef std::list<Entry_KV<K, V>> Bucket;
    // 버킷 배열: 해시 테이블 본체
    typedef std::vector<Bucket>        BktArray;
    typedef typename BktArray::iterator BItor;   // 버킷 이터레이터
    typedef typename Bucket::iterator   EItor;    // 항목(Entry) 이터레이터

protected:
    int         num_entry;  // 전체 저장된 항목 수
    BktArray    BA;         // 해시 테이블 (버킷 배열)
    std::string name;       // 해시 맵 이름

public:
    class Iterator;

public:
    GenericHashMap_KV(std::string nm, int hashTblSize = 101);
    std::string getName() { return name; }
    int  size()  const;
    bool empty() const;
    Iterator find(const K& k);
    Iterator insert(const K& k, const V& v);
    void erase(const K& k);
    void erase(const Iterator& p);
    Iterator begin();
    Iterator end();
    Iterator _find(const K& k);
    Iterator _insert(const Iterator& p, const Entry_KV<K, V>& e);
    void     _erase(const Iterator& p);

    // 버킷 내 다음 항목으로 이동 (항목 이터레이터 직접 증가)
    static void _next(Iterator& p) { ++p.ent; }
    // 버킷의 끝(end)에 도달했는지 확인
    static bool endOfBkt(const Iterator& p) { return p.ent == p.bkt->end(); }

    void fprintBucketSizes(std::ostream& fout);
    // 키 k의 해시 버킷 인덱스 반환
    int  getBucketIndex(const K& k) const
    {
        return static_cast<int>(std::hash<K>{}(k) % BA.size());
    }

public:
    // 해시 맵 내 위치를 가리키는 이터레이터 클래스
    class Iterator {
    private:
        EItor           ent;  // 현재 항목 위치 (버킷 내 리스트 이터레이터)
        BItor           bkt;  // 현재 버킷 위치 (버킷 배열 이터레이터)
        const BktArray* ba;   // 버킷 배열 포인터
    public:
        Iterator() {}
        Iterator(const BktArray& a, const BItor& b, const EItor& q = EItor())
            : ent(q), bkt(b), ba(&a) {
        }
        EItor& getEntry_KV() { return ent; }
        BItor& getBkt() { return bkt; }
        Entry_KV<K, V>& operator*() { return *ent; }
        bool      operator==(const Iterator& p) const;
        bool      operator!=(const Iterator& p) const;
        Iterator& operator++();
        // GenericHashMap_KV 멤버 함수가 ent, bkt에 직접 접근 가능하도록 friend 선언
        friend class GenericHashMap_KV;
    };
};

// 생성자: 버킷 배열을 hashTblSize 크기의 빈 버킷으로 초기화
template <typename K, typename V>
GenericHashMap_KV<K, V>::GenericHashMap_KV(std::string nm, int hashTblSize)
    : num_entry(0), BA(hashTblSize), name(nm)
{
}

// 첫 번째 비어있지 않은 버킷의 첫 항목을 가리키는 Iterator 반환
template <typename K, typename V>
typename GenericHashMap_KV<K, V>::Iterator
GenericHashMap_KV<K, V>::begin() {
    if (empty()) return end();
    BItor bkt = BA.begin();
    while (bkt->empty()) ++bkt;  // 비어있는 버킷 건너뜀
    return Iterator(BA, bkt, bkt->begin());
}

// 해시 테이블 끝을 가리키는 Iterator 반환 (BA.end() 위치)
template <typename K, typename V>
typename GenericHashMap_KV<K, V>::Iterator
GenericHashMap_KV<K, V>::end() {
    return Iterator(BA, BA.end());
}

template <typename K, typename V>
int GenericHashMap_KV<K, V>::size() const { return num_entry; }

template <typename K, typename V>
bool GenericHashMap_KV<K, V>::empty() const { return size() == 0; }

/*
* 함수명 : _find
* 설  명 : 해시 값으로 버킷을 찾고, 버킷 내에서 키 k와 일치하는 항목 위치 반환
*          찾지 못하면 해당 버킷의 end를 가리키는 Iterator 반환
*/
template <typename K, typename V>
typename GenericHashMap_KV<K, V>::Iterator
GenericHashMap_KV<K, V>::_find(const K& k)
{
    //해시 함수로 버킷 인덱스 계산 후 해당 버킷에서 선형 탐색
    std::size_t hashValue = std::hash<K>{}(k) % BA.size();
    BItor bkt = BA.begin() + hashValue;
    Iterator p(BA, bkt, bkt->begin());
    Iterator end(BA, bkt, bkt->end());
    while ((p != end) && (*p).key() != k)
        ++p.ent;
    return p;
}

// _find()로 탐색 후 버킷 끝이면 end() 반환, 아니면 해당 위치 반환
template <typename K, typename V>
typename GenericHashMap_KV<K, V>::Iterator
GenericHashMap_KV<K, V>::find(const K& k)
{
    Iterator p = _find(k);
    if (endOfBkt(p)) return end();
    else             return p;
}

// 버킷의 p 위치 앞에 항목 e를 삽입하고 삽입된 위치의 Iterator 반환
template <typename K, typename V>
typename GenericHashMap_KV<K, V>::Iterator
GenericHashMap_KV<K, V>::_insert(const Iterator& p, const Entry_KV<K, V>& e) {
    EItor ins = p.bkt->insert(p.ent, e);
    num_entry++;
    return Iterator(BA, p.bkt, ins);
}

/*
* 함수명 : insert
* 설  명 : 키 k가 없으면 (k,v)를 새로 삽입, 이미 있으면 기존 값을 v로 교체
*          (Map 동작 - HashDict의 insert와 다름)
*/
template <typename K, typename V>
typename GenericHashMap_KV<K, V>::Iterator
GenericHashMap_KV<K, V>::insert(const K& k, const V& v) {
    Iterator p = _find(k);
    if (endOfBkt(p))
        return _insert(p, Entry_KV<K, V>(k, v));  // 새 항목 추가
    else {
        (p.getEntry_KV())->setValue(v);  // 기존 항목 값 덮어씀
        return p;
    }
}

// 버킷에서 p가 가리키는 항목을 삭제하고 항목 수 감소
template <typename K, typename V>
void GenericHashMap_KV<K, V>::_erase(const Iterator& p) {
    p.bkt->erase(p.ent);
    num_entry--;
}

template <typename K, typename V>
void GenericHashMap_KV<K, V>::erase(const Iterator& p) { _erase(p); }

template <typename K, typename V>
void GenericHashMap_KV<K, V>::erase(const K& k) {
    Iterator p = _find(k);
    if (endOfBkt(p)) std::cout << "Error ::Erase of nonexistent !!\n";
    else             _erase(p);
}

// Iterator 동등 비교: 버킷 배열, 버킷 위치, 항목 위치가 모두 같아야 동등
template <typename K, typename V>
bool GenericHashMap_KV<K, V>::Iterator::operator==(const Iterator& p) const
{
    if (ba != p.ba || bkt != p.bkt) return false;
    else if (bkt == ba->end())      return true;   // 둘 다 전체 끝이면 동등
    else                            return (ent == p.ent);
}

template <typename K, typename V>
bool GenericHashMap_KV<K, V>::Iterator::operator!=(const Iterator& p) const
{
    if (ba != p.ba || bkt != p.bkt) return true;
    else if (bkt == ba->end())      return false;
    else                            return (ent != p.ent);
}

// 다음 항목으로 이동: 버킷 끝 도달 시 다음 비어있지 않은 버킷으로 이동
template <typename K, typename V>
typename GenericHashMap_KV<K, V>::Iterator&
GenericHashMap_KV<K, V>::Iterator::operator++()
{
    ++ent;
    if (endOfBkt(*this)) {
        ++bkt;
        while (bkt != ba->end() && bkt->empty()) ++bkt;  // 빈 버킷 건너뜀
        if (bkt == ba->end()) return *this;
        ent = bkt->begin();
    }
    return *this;
}

/*
* 함수명 : fprintBucketSizes
* 설  명 : 각 버킷의 항목 수를 출력하고 최대/최소/평균 통계를 출력
*          해시 테이블의 부하(load) 분산 상태를 확인하는 데 활용
*/
template <typename K, typename V>
void GenericHashMap_KV<K, V>::fprintBucketSizes(std::ostream& fout)
{
    int max_ent = (int)BA[0].size(), min_ent = max_ent, total = 0;
    int num_bkts = (int)BA.size();

    for (int bkt = 0; bkt < num_bkts; bkt++) {
        int bkt_size = (int)BA[bkt].size();
        fout << "Bucket[" << std::setw(3) << bkt << "] : " << bkt_size << " entries\n";
        if (bkt_size > max_ent) max_ent = bkt_size;
        if (bkt_size < min_ent) min_ent = bkt_size;
        total += bkt_size;
    }
    double avg = static_cast<double>(total) / num_bkts;
    fout.precision(2);
    fout << "Max_ent (" << std::setw(5) << max_ent<< ")"
        << "; min_ent (" << std::setw(5) << min_ent<<")"
        << "; avg (" << std::setw(5) << avg << ")\n";
}
#endif