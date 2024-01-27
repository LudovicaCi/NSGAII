#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

#include "SIMD_function.cpp"

// Definizione di una funzione per determinare se un punto domina un altro punto
// Restituisce true se p è dominato da q
bool domina(const std::vector<double>& punto_q, const std::vector<double>& punto_p) { 

   int conta=0;
   int conta_stretto=0;

   for(int  i=0; i<punto_p.size(); i++){
        
        if(punto_q[i]<=punto_p[i]){
            conta++;
            if(punto_q[i]<punto_p[i]){
                conta_stretto++;
            }
        }
        
   }

   if(conta == punto_p.size() && conta_stretto > 0)
        return true;
        
    return false;
}

// Funzione per dividere una stringa in base a un delimitatore
std::vector<double> split(const std::string &s, char delimiter) {
    std::vector<double> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(std::stod(token));
    }
    return tokens;
}

int main() {

    std::ifstream file("../output_8obj.csv");
    std::vector<std::vector<double>> matrix;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::vector<double> row = split(line, ',');
            matrix.push_back(row);
        }
        file.close();
        
    } else {
        std::cout << "Impossibile aprire il file." << std::endl;
    }

    
    // Definizione delle variabili e inizializzazione
    const int pop_size =  matrix.size();
    std::vector<std::vector<double>> P = matrix;
    std::vector<int> R(pop_size, 0); // vettore contenente il rank del fronte per ogni p
    std::vector<int> F;             // F contine gli indici dei punti appartenenti al fronte corrente
    std::vector<int> allF;          // allF contine gli indici di TUTTI i punti a cui è stato già assegnato un fronte
    int current_front_index = 1; // indice del fronte da considerare nell'iterazione del while

    while (allF.size() < pop_size) {
        for (int p = 0; p < P.size(); ++p) {
            
            // Se findValueInVector restituisce true significa che p è già contenuto in allF
            if (findValueInVector(p,allF,allF.size())) {
                continue;
            }
            int n_p = 0; // domination counter of p (contatore di punti da cui è dominato)
            for (int q = 0; q < P.size(); ++q) {
                if (findValueInVector(q,allF,allF.size())) {
                    continue;
                }
                if (domina(P[q], P[p])) {
                    n_p = n_p + 1; // p è dominato da q
                }
            }

            if (n_p == 0) {
                R[p] = current_front_index;
                F.push_back(p); // aggiungo p alla lista dei punti del fronte corrente
            }
        }
        allF.insert(allF.end(), F.begin(), F.end()); // aggiungo la lista dei punti appartenenti al fronte F alla lista di tutti i fronti
        F.clear();
        current_front_index = current_front_index + 1;
    }

    std::cout << "Fronti trovati: " << current_front_index - 1 << std::endl;

    

    return 0;
}
