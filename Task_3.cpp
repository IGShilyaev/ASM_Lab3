#include <iostream>

using namespace std;

int main()
{
    float x;
    int n;
    float eps;
    bool ok;


    cout << "Value of x: ";
    cin >> x;

    ok = false;
    do 
    {
        cout << "Value of n (positive integer): ";
        cin >> n;
        if (n < 1) cout << "Wrong number, try again!" << endl;
        else ok = true;
    } while (!ok);

    ok = false;
    do
    {
        cout << "Value of eps (non-negative real): ";
        cin >> eps;
        if (eps < 0) cout << "Wrong number, try again!" << endl;
        else ok = true;
    } while (!ok);

    float nSum = 1;
    float epsSum = 1;

    __asm 
    {
        FINIT
        FILD n
        FLD nSum
        FLD x
        CALL nCalc 
        
        FLD epsSum
        FLD eps
        FXCH st(2)
        CALL epsCalc

        jmp fin
     
#pragma region nSum
    nCalc:
        FLD1  //Счетчик операций
        FLD1  //Знаменатель
        FLD1

            ncyc:
        CALL nextElem1
        FLD st(4)
        FADD st(0), st(1)
        FSTP st(5)
        FXCH st(2) //count - в sp(0)
        FLD st(5) //Загружаем n
        FCOMIP st(0),st(1) //Сравнение с модификацией EFLAGS
        je nOutput
        FLD1
        FADDP st(1), st(0)
        FXCH st(2) //count - в sp(2)
        jmp ncyc
        
        
        nOutput:
        FXCH st(4)
        FSTP nSum
        FXCH st(2)
        FFREE st(4)
        FFREE st(3)
        FFREE st(2)
        FFREE st(1)
        RET

        nextElem1:  
        FLD st(3)
        FLD st(0)
        FMULP st(1), st(0)
        FDIV st(0), st(2)
        FLD1
        FADDP st(3), st(0)
        FDIV st(0), st(2)
        FLD1
        FADDP st(3), st(0)
        FCHS
        FMULP st(1), st(0)
        RET
#pragma endregion

#pragma region epsSum

        epsCalc:
        FLD1  //Знаменатель
        FLD1

        epscyc:
        CALL nextElem2
        FLD st(3)
        FLD st(0)
        FADD st(0), st(2)
        FXCH st(1)
        FSUBR st(0), st(1)
        FABS
        FCOMIP st(0), st(6)
        FSTP st(4)
        jbe epsOutput
        jmp epscyc


        epsOutput:
        FXCH st(3)
        FSTP epsSum
        FFREE st(3)
        FFREE st(2)
        FFREE st(1)
        FFREE st(0)
        RET

        nextElem2:
        FLD st(2) // eps || Sum || x || denom || elem || x
        FLD st(0) // eps || Sum || x || denom || elem || x || x
        FMULP st(1), st(0) // eps || Sum || x || denom || elem || x^2
        FDIV st(0), st(2) // eps || Sum || x || denom || elem || x^2 / denom 
        FLD1
        FADDP st(3), st(0) // eps || Sum || x || denom+1 || elem || x^2 / denom
        FDIV st(0), st(2) // eps || Sum || x || denom+1 || elem || x^2 / (denom*(denom+1))
        FLD1
        FADDP st(3), st(0) // eps || Sum || x || denom+2 || elem || x^2 / (denom*(denom+1))
        FCHS 
        FMULP st(1), st(0) // eps || Sum || x || denom+2 || newElem
        RET
#pragma endregion

        fin:
    }


    cout << endl;
    cout << "cos(x):" << endl;
    cout << "Real value: " << cos(x) << endl;
    cout << "Value for n: " << nSum << endl;
    cout << "Value for eps: " << epsSum << endl;

    system("pause");
    return 0;
}


