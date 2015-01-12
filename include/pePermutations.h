#ifndef PE_PERMUTATIONS_H
#define PE_PERMUTATIONS_H

/*
 * Generates the next permutation in lexicographic order
 */
template<typename T>
bool nextLexicalPermutation( T* permutation, int length ) {
    T* ptr = permutation;
    T* k = 0;
    T* l = 0;
    T* final = permutation + length - 1;

    do {
        if ( *ptr < *(ptr + 1) ) k = ptr;
    } while ( ++ptr < final );

    if ( k == 0 ) return false;
    ptr = k + 1;

    do {
        if ( *k < *ptr ) l = ptr;
    } while ( ptr++ < final );

    T temp = *k;
    *k = *l;
    *l = temp;

    ptr = k + 1;
    T* revptr = final;
    do {
        temp = *ptr;
        *ptr = *revptr;
        *revptr = temp;
    } while ( ++ptr < --revptr );
    return true;
}

#endif
