#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric>

// Definizione di una funzione per determinare se un punto domina un altro punto
// Restituisce true se p è dominato da q
bool domina(std::vector<double> punto_q, std::vector<double> punto_p, int n) { 

   int conta=0;
   int conta_stretto=0;

   for(int  i=0; i<n; i++){
        
        if(punto_q[i]<=punto_p[i]){
            conta++;
            if(punto_q[i]<punto_p[i]){
                conta_stretto++;
            }
        }
        
   }

   if(conta == n && conta_stretto > 0)
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

    std::ifstream file("../output.csv");
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
    std::vector<std::vector<double>> P = matrix; // contiene la popolazione letta da file
    std::vector<int> R(pop_size, 0); // vettore contenente il rank del fronte per ogni p
    std::vector<bool> F_bool(pop_size, false); // vale true se il punto corrispondente appartiene al fronte corrente
    std::vector<bool> already_assigned_to_a_front(pop_size, false); // vale true se il punto corrispondente è stato inserito in un fronte precedentemente
    int current_front_index = 1; // indice del fronte da considerare nell'iterazione del while

    while (std::accumulate(already_assigned_to_a_front.begin(), already_assigned_to_a_front.end(), 0) < pop_size) {
        for (int p = 0; p < P.size(); ++p) {
            if (already_assigned_to_a_front[p]){
                continue;
            }
            int n_p = 0; // domination counter of p (contatore di punti da cui è dominato)
            for (int q = 0; q < P.size(); ++q) {
                if (already_assigned_to_a_front[q]) {
                    continue;
                }
                if (domina(P[q], P[p], P[q].size())) {
                    n_p = n_p + 1; // p è dominato da q
                }
            }

            if (n_p == 0) {
                R[p] = current_front_index;
                F_bool[p] = true;
            }
        }

        #pragma clang loop distribute(enable)
        for(int i = 0; i < pop_size; i++){
            already_assigned_to_a_front[i] = F_bool[i];  
        }	

        current_front_index = current_front_index + 1;
    }

    std::cout << "Fronti trovati: " << current_front_index - 1 << std::endl;

    return 0;
}
