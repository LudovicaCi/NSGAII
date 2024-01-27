#include <immintrin.h>
#include <vector>

// La funzione cerca l'intero 'val' all'interno del vettore 'vec' e restituisce un booleano
bool findValueInVector(int val, std::vector<int> vec, int vecSize) {
    // Carica l'intero 'val' in un registro AVX
    __m256i target = _mm256_set1_epi32(val);

    // Scorrimento del vettore in blocchi di 8 elementi (dimensione del registro AVX)
    for (int i = 0; i < vecSize; i += 8) {
        // Carica 8 elementi consecutivi del vettore nell'AVX register
        __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&vec[i]));

        // Confronta gli elementi del vettore con 'val' e ottiene una maschera di confronto
        __m256i compareResult = _mm256_cmpeq_epi32(chunk, target);

        // Verifica se c'è almeno un elemento uguale a 'val' nella maschera di confronto
        int mask = _mm256_movemask_ps(_mm256_castsi256_ps(compareResult));
        if (mask != 0) {
            return true; // Trovato: 'val' è contenuto nel vettore
        }
    }

    return false; // 'val' non è stato trovato nel vettore
}
