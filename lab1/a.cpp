#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <cstdio>
using namespace std;

typedef long double ld;
typedef long long ll;
const ld PI = acos(-1.0);

void plotGraphForC(ld c, ld h, const vector<vector<ld>>& u, const vector<ld>& x) {
    string path = "graphs_a";
    system(("mkdir -p " + path).c_str());

    vector<ld> tList = {0, 2, 4, 5, 7, 9, 10, 11, 13, 15, 16, 18};

    {
        FILE* gp = popen("gnuplot", "w");
        if (!gp) {
            cerr << "Ошибка открытия gnuplot" << endl;
            return;
        }
        char filename[256];
        snprintf(filename, sizeof(filename), (path + "/plot_c_%.2Lf.png").c_str(), c);
        fprintf(gp, "set terminal png size 640,480\n");
        fprintf(gp, "set output \"%s\"\n", filename);
        fprintf(gp, "set title \"График для c = %Lf\"\n", c);
        fprintf(gp, "set xlabel 'x'\n");
        fprintf(gp, "set ylabel 'u(x,t)'\n");
        fprintf(gp, "set key right top\n");
        fprintf(gp, "plot ");
        for (size_t i = 0; i < tList.size(); ++i) {
            fprintf(gp, "'-' with lines title 't = %Lf'", tList[i]);
            if (i != tList.size() - 1)
                fprintf(gp, ", ");
            else
                fprintf(gp, "\n");
        }
        for (auto t : tList) {
            ll index = (ll)(t / (c * h));
            if (index >= u[0].size())
                index = u[0].size() - 1;
            for (size_t i = 0; i < x.size(); ++i) {
                fprintf(gp, "%Lf %Lf\n", x[i], u[i][index]);
            }
            fprintf(gp, "e\n");
        }
        pclose(gp);
    }

    for (auto t : tList) {
        FILE* gp = popen("gnuplot", "w");
        if (!gp) {
            cerr << "Ошибка открытия gnuplot для t = " << t << endl;
            continue;
        }
        char filename[256];
        snprintf(filename, sizeof(filename), (path + "/plot_c_%.2Lf_t_%.2Lf.png").c_str(), c, t);
        fprintf(gp, "set terminal png size 640,480\n");
        fprintf(gp, "set output \"%s\"\n", filename);
        fprintf(gp, "set title \"График для c = %Lf, t = %Lf\"\n", c, t);
        fprintf(gp, "set xlabel 'x'\n");
        fprintf(gp, "set ylabel 'u(x,t)'\n");

        fprintf(gp, "plot '-' with lines title 't = %Lf'\n", t);
        ll index = (ll)(t / (c * h));
        if (index >= u[0].size())
            index = u[0].size() - 1;
        for (size_t i = 0; i < x.size(); ++i) {
            fprintf(gp, "%Lf %Lf\n", x[i], u[i][index]);
        }
        fprintf(gp, "e\n");
        pclose(gp);
    }
}

void solveForC(ll T, ll L, ll NX, ld c, ld h) {
    ll NT = T / (c * h);
    vector<vector<ld>> u(NX, vector<ld>(NT));
    
    vector<ld> x(NX);
    for(size_t i = 0; i < NX; ++i) {
        x[i] = i * h;
    }
    
    for (size_t i = 0; i < NX; ++i) {
        u[i][0] = sin(4 * PI * x[i] / L);
    }

    for (size_t n = 0; n < NT - 1; ++n) {
        for (size_t i = 1; i < NX; ++i) {
            u[i][n + 1] = c * (u[i - 1][n] - u[i][n]) + u[i][n];
        }
        u[0][n + 1] = u[NX - 1][n + 1];
    }

    plotGraphForC(c, h, u, x);
}

int main() {
    vector<ld> C = {0.6, 1, 1.01};
    ll T = 18, L = 20, NX = 41;
    ld h = 0.5;
    for (const auto& c : C) {
        solveForC(T, L, NX, c, h);
    }
}
