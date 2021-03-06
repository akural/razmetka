#pragma once

#include <iostream>
#include <string>
#include "Leads_info.h"
#include "add_structs.h"
#include "functions.h"
#include "pat_storage.h"
#include <iostream>
#include <algorithm>

//using namespace std;

/*! @brief The class provide first analysis of signals  -  finding of peaks.
 *
 */

class one_lead: protected Leads_Info
{
public:
	/**
	 * @param type  - type of lead (I, II, III, aVR, ...
	 * @param N_leads  - number of leads
	 * @param ptr_info_new_peak pount out to object that saves info about currrent new peak
	 */
	one_lead(leads_name type, int N_lead, info_for_new_peak* ptr_info_new_peak);

	virtual ~one_lead() {};

	info_for_new_peak* ptr_info_new_peak;


	//void new_processing(float);
	
	/**
	 * @param peak - type of peaks that is required
	 * @return - a vector with indexes and types of beats (V, N, SV, A)
	 */
	vector< pair<int,  pat_name > >* get_peaks(const char* peak);
	/**
	 *
	 * @param peak  - type of peaks that is required
	 * @return - a vector of required peaks
	 */
	vector<int>* get_peaks(char peak);
	/**
	 *
	 * @param type_signal  - filtered signal or ofigin
	 * @return returns required signal
	 */
	vector<float>* get_signal(char* type_signal);
	/**
	 *
	 * @return Returns start of ventricular pathologies or
	 * 0, when pathology doesn't exist
	 */
	int get_VF(char* );

    /** Provides processing of signals:
     *  - finding of peaks
     *  - analysis of peak's type
     *  - set info about current new peak
     */

	void processing_lead(float& sam);
	/**
	 *
	 * @return a current index of analysing signal
	 */
	int get_count() const;

	/**
	 * @return Returns name of current lead (I, II, III, aVF, ...)
	 */
    leads_name get_lead_name() const;
	
	
	///checking	amplitudes of last 3 peaks because of second peak can be a random disturbance
	bool check_peak_amplitudes_max(int& first_peak, int& second_peak, int& third_peak);
	
	///checking	amplitudes of last 3 peaks because of second peak can be extrasystole with other amplitude
	bool check_peak_amplitudes_min(int& first_peak, int& second_peak, int& third_peak, int type);
	
	void get_ptr_mem(int** sdvig, int** count_iter );

private:
	
	
	
	//storage_of_pathology path_mathods;
	
	//Signal pathology_signal;
	bool finish_vf = 0;
	float last_rhythm = 0.f;
	int old_start_vf = 0;
	int start_vf2 = 0;
	
	int stop_of_R(const int peak, bool type);
	
	///checks a place of extrasystole comparing with last peak R
	bool check_extras(const int& new_peak);
	int start_of_R(const int &peak, float otstup, bool type, vector<float> *use_signal);
	int mem_sdvig = 0;
	int count_iter = 0;
    /**
     * @param peak index of the peak which is required to analyze
     * @param signal points out to signal
     * @return an index of a finish of the peak
     */
    int stop_peak(const int peak, vector<float>* signal);
	
    /**
     * @param otstup defines a possible length of a peak
     * @return a decision about splitting of the peak;
     */
	bool splitting(int peak, vector<float>*signal, const int otstup);
	/**
     * @param peak index of the peak which is required to analyze
     * @param signal points out to signal
     * @return an index of a start of the peak
     */
	int start_peak(const int& peak, vector<float>* signal );

	/**
	 * @brief Finds a ventricular extrasystole between last 3 pealk
	 * @return False, if ventricular extrasystole doesn't exist and true
	 * in another case.
	 */
	bool check_ventr_extrasys( vector<float>*, const int&, int&, const int&);



	//bool check_last_four_peaks( vector< int > &, const int& );

	/**
	 * @brief Sets the initial value of parameters of ECG leads. These values depend on a type of lead
	 * @param leads_name  - type of lead (I,II,III, aVF, ...)
	 *
	 */
	void initialization( leads_name name);
	/**
	 *
	 * @param probably_extrasystole  - index of possible extrasytole
	 * @param peak_for_analysis - last found peak
	 * @return  - decision about an extrasystole
	 */
	bool set_extrasystole( const int& probably_extrasystole, const int & peak_for_analysis);

	/**
	 * @brief This main function contained the algorithm that finds a new_peak
	 * @return true, if a new peak  exists
	 */
	bool finding_of_R(); //true - new peak, false - no new peak;


	void finding_of_P(const int& peak);
	float start_P( vector<float>& bufer, int& ind_start);
	/**
	 * Takes current RR - intervals and checks it.
	 * @param array_of_extrasys  - extrasystoles which stored during finding of R
	 */
	void first_check_of_anormal_RR( vector<int>& array_of_extrasys);

	//variables and functions for  finding R
	 const float limit = 0.0002f;
	//contains signal after differentiation
	 vector <float>  after_diff_signal;

	 //these vectors storage some values for R-detection algorithm
	 vector<float> array_of_pol, vect_of_2part,filter_signal_pol;
     int st = 0;
	 int last_peak_outlier = 0;
	 float S_amplitude = 0.f;
	//Provides such operations as derivatig and filtering
	void deriv_of_signal(float&);
	void filter( vector<float>&,  vector<float>&, int);

	//additional vector for filtering
	 vector<float>z;
	 bool high_rhythm  = false;


	vector< pair<int, pat_name>> peaks_with_types; //Contains peaks with types (N, V and so on)
	vector<int>extrasystoles; 					   //for all extrasystoles of lead
	//provide finding R(S) peaks to future analysis
	bool first_check_peak_S();
	bool check_peak();

	//provide analysis of the type current new peak (N,V ...)
	void testing_of_RR();
	void testing_of_SS();



	/*some_sometimes there'ra different type of wave P - it can be as usual, in
	can be inverted or absented - 1,-1 or 0 */
	int type_of_P(int start_peak);
	
	bool check_V_forme(vector<pair<int, pat_name>>& peaks, int average_RR);

	/*for ventricular flutter.
	 * The function provides finding of R;
	 * vectors storage list of peaks and their amplitudes
	 * Variable start_vf saves index of start current episode of ventricular flutter or nothing (0)
	 */
	bool ventr_flutter(size_t st_f, const int& len_st);
	vector<int>list_extrasys;
	vector<float>list_ampl;
	int start_vf = 0;
	float diff_last_peak = 0.f;
	float param_limit = 0.f;

};

