// please see the explanation in the documentation
// http://www.resibots.eu/limbo

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
using namespace limbo;


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
        BO_PARAM(int, samples, 5);
    };

    // we stop after 40 iterations
    struct stop_maxiterations {
        BO_PARAM(int, iterations, 30);
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
        for(int j=0; j < 3; j++){
            sumalphas = alphas[j];
        }
        for(int j=0; j < 3; j++){
            alphas[j] = alphas[j]/sumalphas;
        }


            // YOUR CODE HERE
        std::cout << "Valores Orig - m="<<x(0) << " | p_Tresh=" << x(1) << " | pr=" << x(2) << std::endl;
        std::cout << "Valores Esca - m="<<((((int)(x(0)*20))*2)+10) << " | p_Tresh=" << ((x(1)*(0.5))+0.01) << " | pr=" << ((x(2)*(0.5))+0.01) << std::endl;
        std::vector<float> fitness_ga;
        std::vector<int> it_ga;
        float fitn;
        int it;
        for (int j=0; j<5; j++){
            std::tie(fitn,it) = coalicionGM(((( (int)(x(0)*20)  )*2)+10), ((x(1)*(0.5))+0.01), ((x(2)*(0.2))+0.01));
            std::cout << "Iteracion: " << j << " Fitness: " << fitn << " iteraciones: " << it << std::endl;
            fitness_ga.push_back(fitn);
            it_ga.push_back(it);            
        }
        // Obtiene el min fitness_ga
        float minFitnees_ga= *std::min_element(fitness_ga.begin(),fitness_ga.end());
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
        float solution = minFitnees_ga*alphas[0]+sdFitness_ga*alphas[1]+tmean*alphas[2];
        return tools::make_vector(-solution);
    }
};


int main()
{
    // run the evaluation
    bayes_opt::BOptimizer<Params> boptimizer;
    // run the evaluation
    boptimizer.optimize(eval_func<Params>());
    // the best sample found
    std::cout << "Best sample: " << boptimizer.best_sample()(0) << " - Best observation: " << boptimizer.best_observation()(0) << std::endl;
    return 0;
}

