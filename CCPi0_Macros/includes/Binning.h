#ifndef Binning_h
#define Binning_h

#include "TArrayD.h"
#include "TMath.h"   // Sort



// Declare binning functions
// =========================

TArrayD GetTArrayFromVec(const std::vector<double>& vec);

void SortArray(TArrayD& arr);

TArrayD GetSortedArray(const TArrayD& arr);

TArrayD MakeUniformBinArray(int nbins, double min, double max);

std::vector<double> GetVecFromArray(const TArrayD& arr);





// ==========================================================================
//  Namespace with vector of bin limits
//  (Useful mostly for variables with variable-size bins)
// ==========================================================================

namespace CCPi0
{
    TArrayD GetBinning(const std::string var_name)
    {
        std::vector<double> bins_vec;
        
        
        // Cross-section variables
        // =======================
        
        if ( var_name == "MuonPt" )  // [GeV/c]
            bins_vec = {0., 0.075, 0.15, 0.25, 0.325, 0.4, 0.475, 0.55, 0.7, 0.85, 1.0, 1.25, 1.5, 2.5};  // Dan's CCQENu
        
        
        // Track variables
        // ===============
        
        else if ( var_name == "TrackPionScore" )
            bins_vec = {-60., -50., -40., -35., -30., -25., -20., -15., -10, -7.5, -5., -2.5, 0., 2., 4., 6., 8., 10.,
                        12., 14., 16., 18., 20., 22.5, 25., 27.5, 30., 35., 40., 50.};
        
        
        // Blob variables
        // ==============
        
        else if ( var_name == "BlobAngleWRTMuon" )  // [deg]
            bins_vec = {0., 3., 6., 9., 12., 15., 18., 21., 24., 27., 30., 33., 36., 39., 42., 45., 48., 51., 54., 57., 60.,
                        65., 70., 75., 80., 85., 90., 95., 100., 110., 120., 130., 140., 150., 160., 180.};
        
        else if ( var_name == "BlobTheta" )  // [deg]
            bins_vec = {0., 3., 6., 9., 12., 15., 18., 21., 24., 27., 30., 33., 36., 39., 42., 45., 48., 51., 54., 57., 60.,
                        65., 70., 75., 80., 85., 90., 95., 100., 110., 120., 130., 140., 150., 160., 180.};
        
        else if ( var_name == "BlobProjDeviation" )  // [mm]
            bins_vec = {0., 20., 40., 60., 80., 100., 120., 140., 160., 180., 200., 225., 250., 275., 300., 350., 400., 450., 500.,
                        550., 600., 650., 700., 800., 900., 1000., 1200., 1500.};
        
        else if ( var_name == "BlobAngleDeviation" )  // [deg]
            bins_vec = {0., 3., 6., 9., 12., 15., 18., 21., 24., 27., 30., 33., 36., 39., 42., 45., 48., 51., 54., 57., 60.,
                        65., 70., 75., 80., 85., 90., 95., 100., 110., 120., 130., 140., 150., 160., 180.};
        
        else if ( var_name == "BlobEcalo" )  // [MeV]
            bins_vec = {0., 20., 40., 60., 80., 100., 120, 140., 160., 185., 200., 220., 240., 260., 280., 300.,
                        320., 340., 360., 380., 400., 450., 500., 550., 600., 700., 800., 1000., 1200.};
        
        else if ( var_name == "Blobdx" )  // [cm]
            bins_vec = {0., 2., 4., 6., 8., 10., 12., 14., 16., 18., 20., 22., 24., 26., 28., 30., 32., 34., 36., 38., 40.,
                        42., 44., 48., 50., 55., 60., 65., 70., 75., 80., 85., 90., 100., 110., 120.};
        
        else if ( var_name == "BlobdEdxMean" )  // [MeV/cm]
            bins_vec = {0., 0.25, 0.5, 0.75, 1., 1.25, 1.5, 1.75, 2., 2.25, 2.5, 2.75, 3., 3.25, 3.5, 3.75, 4., 4.25, 4.5, 4.75, 5.,
                        5.25, 5.5, 5.75, 6., 6.25, 6.5, 6.75, 7., 7.5, 8., 8.5, 9., 9.5, 10., 11., 12., 13., 14., 15., 16., 18., 20., 22.};
        
        else if ( var_name == "BlobdEdxFront" )  // [MeV/cm]
            bins_vec = {0., 0.25, 0.5, 0.75, 1., 1.25, 1.5, 1.75, 2., 2.25, 2.5, 2.75, 3., 3.25, 3.5, 3.75, 4., 4.25, 4.5, 4.75, 5.,
                        5.25, 5.5, 5.75, 6., 6.25, 6.5, 6.75, 7., 7.5, 8., 8.5, 9., 9.5, 10., 11., 12., 13., 14., 15., 16., 18., 20., 22.};
        
        else if ( var_name == "BlobdEdxEnd" )  // [MeV/cm]
            bins_vec = {0., 0.25, 0.5, 0.75, 1., 1.25, 1.5, 1.75, 2., 2.25, 2.5, 2.75, 3., 3.25, 3.5, 3.75, 4., 4.25, 4.5, 4.75, 5.,
                        5.25, 5.5, 5.75, 6., 6.25, 6.5, 6.75, 7., 7.5, 8., 8.5, 9., 9.5, 10., 11., 12., 13., 14., 15., 16., 18., 20., 22.};
        
        
        // Energy variables
        // ================
        
        else if ( var_name == "NoPi0RecoilE" )  // [GeV]
            bins_vec = {0., 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.2, 1.5, 2.0};
        
        else if ( var_name == "RecoilE" )  // [GeV]
            bins_vec = {0., 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.5, 3.0, 4.0, 6.0};
        
        else if ( var_name == "Q2" )  // [GeV^2]
            bins_vec = {0., 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7, 0.8, 0.9, 1., 1.2, 1.4, 1.6, 1.8, 2., 2.5, 3., 4.};
        
        else if ( var_name == "W2" )  // [GeV^2/c^4]
            bins_vec = {0., 0.2, 0.4, 0.6, 0.8, 1., 1.2, 1.4, 1.6, 1.8, 2., 2.2, 2.4, 2.6, 2.8, 3., 3.2, 3.4, 3.6, 3.8, 4.,
                        4.4, 4.8, 5.2, 5.6, 6., 6.5, 7., 8.};
        
        else if ( var_name == "W" )  // [GeV/c^2]
            bins_vec = {-0.5, 0., 0.2, 0.4, 0.6, 0.8, 1., 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.,
                        2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3., 3.2, 3.4, 3.6, 4.};
        
        
        // Alternative efficiency variables
        // ================================
        
        else if ( var_name == "MuonPz" )  // [GeV/c]
            bins_vec = {0., 1.5, 2., 3., 3.5, 4., 4.5, 5., 6., 7., 8., 9., 10., 15., 20.};  // Dan's CCQENu
        
        else if ( var_name == "Pi0E" )  // [GeV]
            bins_vec = {0., 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 1., 1.2, 1.6, 2., 2.5, 3.};
        
        else if ( var_name == "Pi0KE" )  // [GeV]
            bins_vec = {0., 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 1., 1.2, 1.6, 2., 2.5, 3.};
        
        else if ( var_name == "Pi0P" )  // [GeV/c]
            bins_vec = {0., 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 1., 1.2, 1.6, 2., 3.};
        
        else if ( var_name == "Pi0Theta" )  // [deg]
            bins_vec = {0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 120., 150., 180.};
        
        
        // Prepare an array from the bin vector
        TArrayD bins_array(GetTArrayFromVec(bins_vec));
        SortArray(bins_array);
        
        return bins_array;
    }
}  // namespace CCPi0





// ==========================================================================
//  Binning functions
// ==========================================================================

// Return a TArray from a vector
// =============================

TArrayD GetTArrayFromVec(const std::vector<double>& vec)
{
    double array[vec.size()];
    std::copy(vec.begin(), vec.end(), array);
    const int size = sizeof(array) / sizeof(*array);
    
    return TArrayD(size, array);
}



// Sort TArray in increasing order
// ===============================

void SortArray(TArrayD& arr)
{
    // Store in 'index' the correct order of array
    int size = arr.GetSize();
    int* index = new int[size];
    TMath::Sort(size, arr.GetArray(), index, kFALSE);  // Default ordering is decreasing, set 'kFALSE' for increasing ordering
    
    // Then make a new array from that correct ordering
    TArrayD dummy(size);
    for ( int i = 0; i < size; ++i ) dummy[i] = arr[index[i]];
    
    // Set sorted array
    arr = dummy;
}



// Get TArray sorted in increasing order
// =====================================

TArrayD GetSortedArray(const TArrayD& arr)
{
    // Store in 'index' the correct order of array
    int size = arr.GetSize();
    int* index = new int[size];
    TMath::Sort(size, arr.GetArray(), index, kFALSE);  // Default ordering is decreasing, set 'kFALSE' for increasing ordering
    
    // Then make a new array from that correct ordering
    double sorted_array[size];
    for ( int i = 0; i < size; ++i ) sorted_array[i] = arr[index[i]];
    
    // Return sorted array
    return TArrayD(size, sorted_array);
}



// Get a TArray of uniform bins from a given minimum, maximum and number of bins
// =============================================================================

TArrayD MakeUniformBinArray(int nbins, double min, double max)
{
    double step_size = (max - min) / nbins;
    double arr[nbins + 1];  // +1 because binning arrays include top edge
    
    for ( int i = 0; i <= nbins; ++i ) arr[i] = min + (i*step_size);
    const int size = sizeof(arr) / sizeof(*arr);
    
    return TArrayD(size, arr);
}



// Get a vector from a sorted TArray in increasing order
// =====================================================

std::vector<double> GetVecFromArray(const TArrayD& arr)
{
    // Store in 'index' the correct order of array
    int size = arr.GetSize();
    int* index = new int[size];
    TMath::Sort(size, arr.GetArray(), index, kFALSE);  // Default ordering is decreasing, set 'kFALSE' for increasing ordering
    
    // Create vector from sorted array
    std::vector<double> vec;
    for ( int i = 0; i < size; ++i ) vec.push_back(arr[index[i]]);
    
    // Return vector
    return vec;
}


#endif  // Binning_h