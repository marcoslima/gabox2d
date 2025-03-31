#ifndef __LMFISICA_H__
#define __LMFISICA_H__
// M�dulo de f�sica da LibMarcos: LmFisica
////////////////////////////////////////////

#include "lmmath.h"
#include <stdexcept>
#include <list>

// Tipo doublex: double com incerteza
#include "doublex.h"

namespace LmFisica
{
    double Round(double num, int nCasas = 0);

    // Funções de cálculos muito utilizados:
    // Quadratura:
    inline double quadratura(const double& a, const double& b)
    {
        return std::sqrt(a * a + b * b);
    }

    // Compatibilidade de valores com incertezas diferentes:
    inline bool compativel(const double& dVal1, const double& dVal2,
                           const double& dIncerteza1, const double& dIncerteza2)
    {
        return std::abs(dVal1 - dVal2) / quadratura(dIncerteza1, dIncerteza2) <= 3;
    }

    using vec_dbl_t = std::vector<double>;
    using vec_pair_dbl_t = vector<pair<double, double> > ;

    // Soma dos elementos
    template<class T>
    T Somatorio(const vector<T> &vecValores)
    {
        T aTot(0);
        const size_t nSize = vecValores.size();

        for (size_t i = 0; i < nSize; i++)
        {
            aTot += vecValores[i];
        }

        return aTot;
    }

    // Soma dos elementos ao quadrado (cada um ao quadrado, e então somados)
    template<class T>
    T Somatorio2(vector<T> &vecValores)
    {
        T aTot(0);
        const size_t nSize = vecValores.size();

        for (size_t i = 0; i < nSize; i++)
        {
            aTot += vecValores[i] * vecValores[i];
        }

        return aTot;
    }

    template<class T, class R>
    R Media(const vector<T> &vec)
    {
        return static_cast<R>(Somatorio<T>(vec)) / vec.size();
    }

    // Desvio padrão:
    template<class T, class R>
    R StdDev(const vector<T> &vec)
    {
        R aMed = Media<T, R>(vec);
        R aTot(0.0);
        const size_t nSize = vec.size();

        if (nSize <= 1)
            return 0;

        for (size_t i = 0; i < nSize; i++)
        {
            aTot += (vec[i] - aMed) * (vec[i] - aMed);
        }

        return static_cast<R>(::sqrt(static_cast<R>(aTot / static_cast<R>(nSize - 1))));
    }

    template<class T, class R>
    R Mediana(const vector<T> &vec)
    {
        list<T> lst;
        typename vector<T>::size_type i, size = vec.size();
        switch (size)
        {
            case 0:
                return static_cast<R>(0);
            case 1:
                return static_cast<R>(vec[0]);
            case 2:
                return static_cast<R>((vec[0] + vec[1]) / 2.0);
            default:
                break;
        }
        // Se chegar aqui, o tamanho é maior que 2:

        for (i = 0; i < size; ++i)
        {
            lst.push_back(vec[i]);
        }
        lst.sort();

        if (size % 2 == 0)
        {
            size /= 2;
            return static_cast<R>((vec[size] + vec[size + 1]) / 2.0);
        }
        size = (vector<T>::size_type)((size - 1) / 2.0) + 1;
        return static_cast<R>(vec[size]);
    }

    // Media ponderada: recebe vetor com valores (v = vecValores[i], e vetor com pesos (p = vecPesos[i])
    //                                             i                                     i
    /*
             n
            ====
            \
             >    p  v
            /      i  i
            ====
            i = 1
    mp = ------------------
             n
            ====
            \
             >    p
            /      i
            ====
            i = 1
    */
    template<class T, class R>
    R MediaPonderada(vector<T> &vecValores, vector<T> &vecPesos)
    {
        R aTot(0);
        size_t nSize = vecValores.size();
        if (vecPesos.size() != nSize)
        {
            throw invalid_argument("Vetor de pesos tem tamanho diferente do vetor de valores");
        }

        for (size_t i = 0; i < nSize; i++)
        {
            aTot += vecValores[i] * vecPesos[i];
        }

        return aTot / Somatorio<T>(vecPesos);
    }

    // domingo, 13 de dezembro de 2009 10:16:13 StdPonderada parece estar incompleta
    template<class T, class R>
    R StdevPonderada(vector<T> &vecValores, vector<T> &vecPesos)
    {
        R aTot(0);
        size_t nSize = vecValores.size();
        if (vecPesos.size() != nSize)
        {
            throw invalid_argument("Vetor de pesos tem tamanho diferente do vetor de valores");
        }

        for (size_t i = 0; i < nSize; i++)
        {
            aTot += vecValores[i] * vecPesos[i];
        }

        return aTot / Somatorio<T>(vecPesos);
    }

    void MediaPonderada(vector<double> &vecValores, vector<double> &vecIncertezas,
                        double &outMedia, double &outIncerteza);

    // Obtém Média e Desvio padrão de um std::vector, ambos num tipo indicado (class R):
    template<class T, class R>
    void Stats(vector<T> &vecValores, R &retMedia, R &retStdDev)
    {
        retMedia = Media<T, R>(vecValores);
        retStdDev = StdDev<T, R>(vecValores);
    }

    // Mantém apenas algarismos significativos:
    int Significativos(double &grandeza, double &incerteza, bool cientifico = false);

    double SigniVal(const double &aV, const double &aS);

    double SigniS(const double &aS);

    // Regressão linear simples
    void Inclinacao(vec_pair_dbl_t vecValores, double &a, double &b);


    class CAngulo
    {
        doublex m_Val; // radianos
        doublex m_degVal; // Graus
    public:
        doublex GetDeg();

        doublex GetRad();

        void SetDeg(const doublex &aDeg);

        void SetRad(const doublex &aRad);

        doublex GetSin();

        doublex GetCos();

        doublex GetTan();
    };
} // namespace LmFisica
#endif //__LMFISICA_H__
