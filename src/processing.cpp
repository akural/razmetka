
#include <algorithm>
#include "one lead.h"
#include "Leads_info.h"
#include "All_leads.h"
#include "LeadII_V.h"

//extern float fusion_window_sec;

void one_lead::processing_lead(float& sam)
{
	
	static int N = 0;
	if (N_leads == N) 
	{
		N = 0;
		array_of_peak_R.reserve(n_peaks);
		peaks_with_types.reserve(n_peaks);
		All_Leads::count_leads++;
	} else
		N++;

	static int first = 0;
	if (first<N_leads)
	{
		initialization(type_of_lead); 
		first++;
	}
	if (type_of_lead == II)
	{
		ptr_info_new_peak->increment_time(Fs);
	}

	count++;

	try{
		deriv_of_signal(sam);
		
		if ( (count > n) && finding_of_R())
		{
			if (new_peak != last_peak)
			{
				last_peak = new_peak;
					
				if (pathology_signal.VFib == 0)
				{

					if (main_peak_in_interval =='R')
					{
						if (check_peak())
						{
							testing_of_RR();
							bool res = check_V_forme(peaks_with_types, average_R);
							if (array_of_peak_R.size()>2 && !ptr_info_new_peak->get_R_is() &&
									ptr_info_new_peak->get_sample() <  (peaks_with_types.end() - 3)->first)
							{
        
								ptr_info_new_peak->set_is_peak(true);
								ptr_info_new_peak->set_sample((peaks_with_types.end() - 3)->first);
								ptr_info_new_peak->set_is_peak(true);
								
								finding_of_P(*(array_of_peak_R.end() - 3));
							}
							if (array_of_peak_R.size() > 2)
							{
								print((peaks_with_types.end() - 3)->first, "main_peaks.txt");
								//print(*(array_of_peak_R.end() - 3), "main_peaks.txt");
							}


						}

					} /*else
						if (first_check_peak_S()) 
						{
							testing_of_SS();
							if (array_of_peak_R.size()>2 && !ptr_info_new_peak->get_R_is())
							{
								ptr_info_new_peak->set_is_peak(true);
								//print(*(array_of_peak_R.end() - 3), "main_peaks.txt");
								ptr_info_new_peak->set_sample(*(array_of_peak_R.end() - 3));
								ptr_info_new_peak->set_is_peak(true);

								//for finding of original isolinia;
								finding_of_P(*(array_of_peak_R.end() - 3));
							}
							if (array_of_peak_R.size() > 2)
							{
								print(*(array_of_peak_R.end() - 3), "main_peaks.txt");
							}
						}*/
					
				}
			}
            int last_peak_asystol = 0;
			if (!peaks_with_types.empty())
			    last_peak_asystol = (peaks_with_types.end()-1)->first;

			if (!extrasystoles.empty() && *(extrasystoles.end()-1) > last_peak_asystol )
			    last_peak_asystol =  *(extrasystoles.end()-1);

            if (count - last_peak_asystol > static_cast<int>(5 * Fs)) {
                pathology_signal.Asystol++;
            }
            if (pathology_signal.Asystol > 0 && count - last_peak_asystol < static_cast<int>(5 * Fs)) {
                pathology_signal.Asystol = 0;
            }
		}
		
	} catch (const  exception& oor) {
		 cerr << "Out of Range error: " << oor.what() << " count and type " << count << " "<<type_of_lead <<'\n';
		exit(1);

	}

}


