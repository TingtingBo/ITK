/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorConfidenceConnectedImageFilterTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <fstream>
#include "itkVectorConfidenceConnectedImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTextOutput.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkFilterWatcher.h"
#include "itkRGBPixel.h"

int itkVectorConfidenceConnectedImageFilterTest(int ac, char* av[] )
{
  // Comment the following if you want to use the itk text output window
  itk::OutputWindow::SetInstance(itk::TextOutput::New());

  if(ac < 5)
    {
    std::cerr << "Usage: " << av[0] << " InputImage BaselineImage seedX seedY\n";
    return -1;
    }

  const unsigned int Dimension = 2;

  typedef unsigned char PixelComponentType;
  typedef itk::RGBPixel<PixelComponentType> PixelType;

  typedef unsigned char OutputPixelType;

  typedef itk::Image<PixelType,       Dimension> ImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  
  typedef itk::ImageFileReader<ImageType>  ReaderType;
    
   ReaderType::Pointer input = ReaderType::New();
  input->SetFileName(av[1]);
  
  // Create a filter
  typedef itk::VectorConfidenceConnectedImageFilter<
                                              ImageType,
                                              OutputImageType
                                                > FilterType;

  FilterType::Pointer filter = FilterType::New();
  FilterWatcher filterWatch(filter);

  filter->SetInput(input->GetOutput());
  filter->SetInitialNeighborhoodRadius( 3 ); // measured in pixels

  FilterType::IndexType seed; 
  
  seed[0] = atoi( av[3] );
  seed[1] = atoi( av[4] );
  
  filter->SetSeed(seed);
  filter->SetMultiplier( 2.5 );
  filter->SetReplaceValue( 255 );
  filter->SetNumberOfIterations( 5 );

  try
    {
    input->Update();
    filter->Update();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception detected: "  << e.GetDescription();
    return -1;
    }

  // Test the GetMacros
  double doubleMultiplier = filter->GetMultiplier();
  std::cout << "filter->GetMultiplier(): " 
            << doubleMultiplier
            << std::endl;

  unsigned int uintNumberOfIterations = filter->GetNumberOfIterations();
  std::cout << "filter->GetNumberOfIterations(): "
            << uintNumberOfIterations 
            << std::endl;

  OutputPixelType pixelReplaceValue = filter->GetReplaceValue();
  std::cout << "filter->GetReplaceValue(): "
            << static_cast<itk::NumericTraits<OutputPixelType>::PrintType>(pixelReplaceValue)
            << std::endl;

  const unsigned int cuintInitialNeighborhoodRadius = filter->GetInitialNeighborhoodRadius();
  std::cout << "filter->GetInitialNeighborhoodRadius(): "
            << cuintInitialNeighborhoodRadius
            << std::endl;

  // Generate test image
  typedef itk::ImageFileWriter<OutputImageType>  WriterType;
  WriterType::Pointer writer = WriterType::New();
    
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( av[2] );
  writer->Update();



  // Exercise AddSeed() method
  filter->AddSeed( seed );


    return 0;
}
