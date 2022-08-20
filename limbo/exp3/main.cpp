
#include <iostream>

// you can also include <limbo/limbo.hpp> but it will slow down the compilation
#include <limbo/acqui/gp_ucb.hpp>
#include <limbo/bayes_opt/boptimizer.hpp>
#include <limbo/kernel/matern_five_halves.hpp>
#include <limbo/mean/data.hpp>
#include <limbo/model/gp.hpp>
#include <limbo/stat.hpp>
#include <limbo/tools/macros.hpp>
#include <coalicionGM.hpp>
#include <set>
#include <tuple>
#include <stdio.h>
#include <sstream>
#include <random>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <string>
using namespace limbo;


int count_it = 0;
/*
* Prepara archivos para guardar los datos de cada ejecución
*/

std::ofstream info;
std::string infotxt = "info_data.txt"; 

/*
* Genera el archivo que almacena los datos de la solución
*/
std::ofstream info_group;
std::string nameinfo_test = "info_group_data.txt"; 


struct Params {
    struct bayes_opt_boptimizer : public defaults::bayes_opt_boptimizer {
    };

// depending on which internal optimizer we use, we need to import different parameters
#ifdef USE_NLOPT
    struct opt_nloptnograd : public defaults::opt_nloptnograd {
    };
#elif defined(USE_LIBCMAES)
    struct opt_cmaes : public defaults::opt_cmaes {
    };
#else
    struct opt_gridsearch : public defaults::opt_gridsearch {
    };
#endif

    // enable / disable the writing of the result files
    struct bayes_opt_bobase : public defaults::bayes_opt_bobase {
        BO_PARAM(int, stats_enabled, true);
    };

    // no noise
    struct kernel : public defaults::kernel {
        BO_PARAM(double, noise, 1e-10);
    };

    struct kernel_maternfivehalves : public defaults::kernel_maternfivehalves {
    };

    // we use 10 random samples to initialize the algorithm
    struct init_randomsampling {
        BO_PARAM(int, samples, 10);
    };

    // we stop after 40 iterations
    struct stop_maxiterations {
        BO_PARAM(int, iterations, 150);
    };

    // we use the default parameters for acqui_ucb
    struct acqui_ucb : public defaults::acqui_ucb {
    };
};


template <typename Params>
struct eval_func {
    // number of input dimension (x.size())
    BO_PARAM(size_t, dim_in, 3);
    // number of dimensions of the result (res.size())
    BO_PARAM(size_t, dim_out, 1);
    
    // the function to be optimized
    Eigen::VectorXd operator()(const Eigen::VectorXd& x) const
    {
        /// Genera alphas
        float alphas[3] = {30,20,10};
        float sumalphas = 0;
        for(int j=0; j <3 ; j++){
            sumalphas += alphas[j];
        }
        for(int j=0; j < 3; j++){
            alphas[j] = alphas[j]/sumalphas;
        }

        int m_val =  ((( (int)(x(0)*25)  )*2)+10); // Valores entre 10 a 60
        float p_thres = ((x(1)*(0.59))+0.01);  // Valores entre 0.2 a 0.6
        float p_r = ((x(2)*(0.399))+0.001); // Valores entre 0.001 a 0.4
        std::cout << "Valores Orig - m="<<x(0) << " | p_Tresh=" << x(1) << " | pr=" << x(2) << std::endl;
        std::cout << "Valores Esca - m="<<m_val << " | p_Tresh=" << p_thres << " | pr=" << p_r << std::endl;
        std::vector<float> fitness_ga;
        std::vector<int> it_ga;
        float fitn;
        int it;

        for (int j=0; j<50; j++){
            std::tie(fitn,it) = coalicionGM(m_val, p_thres , p_r);
            
            std::cout << "Iteracion: " << j << " Fitness: " << fitn << " iteraciones: " << it << std::endl;
            info << std::fixed << fitn << std::setprecision(20)  << "," 
                << it << "," 
                << count_it << "," 
                << j << "," 
                << m_val << "," 
                << p_thres << "," 
                << p_r << "," 
                << x(0) << "," 
                << x(1) << "," 
                << x(2) << std::endl;
            fitness_ga.push_back(fitn);
            it_ga.push_back(it);            
        }
        // Obtiene el min fitness_ga
        float minFitness_ga= *std::min_element(fitness_ga.begin(),fitness_ga.end());
        // Obtiene el min iteraciones
        int sumT = std::accumulate(it_ga.begin(),it_ga.end(),0);
        float tmean = sumT/ it_ga.size();
        // Calcula la varianza
        float sumFit = std::accumulate(fitness_ga.begin(),fitness_ga.end(),0.0);
        float meanFi = sumFit/fitness_ga.size();
        float var = 0;
        for (int j=0; j < fitness_ga.size(); j++){
            var += (fitness_ga[j]-meanFi)*(fitness_ga[j]-meanFi);
        }
        var /= fitness_ga.size();
        float sdFitness_ga = sqrt(var);
            // return a 1-dimensional vector
        float solution = minFitness_ga*0.99+sdFitness_ga*0.009+tmean*0.001;
        info_group << std::fixed << -solution << std::setprecision(20) << ","
            << minFitness_ga << "," 
            << sdFitness_ga << "," 
            << tmean << "," 
            << count_it << ","
            << 0.99 << ","
            << 0.009 << ","
            << 0.001 << std::endl;
        count_it++;
        return tools::make_vector(-solution);
    }
};


int main()
{
    char timestr[20];
    std::string prefijo_save;
    time_t hora_actual;
    struct tm * time_info;
    time(&hora_actual);
    time_info = localtime(&hora_actual);
    strftime(timestr, sizeof(timestr), "%Y-%m-%d T:%H-%M", time_info);
    prefijo_save = std::string(timestr);
    info.open(prefijo_save+"-"+infotxt);
    info_group.open(prefijo_save+"-"+nameinfo_test);

    // run the evaluation
    bayes_opt::BOptimizer<Params> boptimizer;
    // run the evaluation
    info << "fitnes"  << "," 
            << "iter" << "," 
            << "n_group" << "," 
            << "n_execution" << "," 
            << "m" << "," 
            << "p_thresh" << "," 
            << "p_r" << "," 
            << "x1" << "," 
            << "x2" << "," 
            << "x3" << std::endl;
    info_group << "solution" << ","
        << "minFitness" << "," 
        << "sdFitness" << "," 
        << "tmean" << "," 
        << "iter" << ","
        << "alphas_1" << ","
        << "alphas_2" << ","
        << "alphas_3" << std::endl;

    boptimizer.optimize(eval_func<Params>());
    // the best sample found
    std::cout << "Best sample: " << boptimizer.best_sample()(0) << " - Best observation: " << boptimizer.best_observation()(0) << std::endl;
    info.close();
    info_group.close();
    return 0;
}

