#ifndef PE_COMBINATIONS_H
#define PE_COMBINATIONS_H
/*
 * Generates the next combination of elements from "max Choose index"
 * index is so named because alterations start at (combination + index)
 * the rest of the array past that element is untouched
 */
bool nextSeqNumericCombination( int* combination, int index, int max ) {
    int i = index - 1;
    combination[i]++;
    while ( (i > 0) && (combination[i] >= (max + 1 - index) + 1 + i) ) {
        i--;
        combination[i]++;
    }

    if (combination[0] > max + 1 - index ) return false;

    for ( int j = i + 1; j < index; j++ ) {
        combination[j] = combination[j - 1] + 1;
    }
    return true;
}
#endif
