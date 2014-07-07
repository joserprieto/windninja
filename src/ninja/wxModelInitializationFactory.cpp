/******************************************************************************
 *
 * $Id$
 *
 * Project:  WindNinja
 * Purpose:  Implementation file for wxModelInitialization Factory Class
 * Author:   Levi Malott <lmnn3@mst.edu> 
 *
 ******************************************************************************
 *
 * THIS SOFTWARE WAS DEVELOPED AT THE ROCKY MOUNTAIN RESEARCH STATION (RMRS)
 * MISSOULA FIRE SCIENCES LABORATORY BY EMPLOYEES OF THE FEDERAL GOVERNMENT
 * IN THE COURSE OF THEIR OFFICIAL DUTIES. PURSUANT TO TITLE 17 SECTION 105
 * OF THE UNITED STATES CODE, THIS SOFTWARE IS NOT SUBJECT TO COPYRIGHT
 * PROTECTION AND IS IN THE PUBLIC DOMAIN. RMRS MISSOULA FIRE SCIENCES
 * LABORATORY ASSUMES NO RESPONSIBILITY WHATSOEVER FOR ITS USE BY OTHER
 * PARTIES,  AND MAKES NO GUARANTEES, EXPRESSED OR IMPLIED, ABOUT ITS QUALITY,
 * RELIABILITY, OR ANY OTHER CHARACTERISTIC.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/


#include "wxModelInitializationFactory.h"

/**
 * Create a wxInitialization based on the type of forecast
 *
 * @param fileName
 *
 * @return an initializer for windninja
 */
wxModelInitialization* wxModelInitializationFactory::makeWxInitialization( std::string fileName )
{
    if(fileName.empty())
    throw std::runtime_error("The weather model initialization filename has not been set.");

    ncepNdfdInitialization ncepNdfd;
    ncepNamSurfInitialization ncepNamSurf;
    ncepRapSurfInitialization ncepRapSurf;
    ncepDgexSurfInitialization ncepDgexSurf;
    ncepNamAlaskaSurfInitialization ncepNamAlaskaSurf;
    ncepGfsSurfInitialization ncepGfsSurf;
    genericSurfInitialization genericSurf;
    wrfSurfInitialization wrfSurf;
    wrf3dInitialization wrf3d;
    ncepNamGrib2SurfInitialization ncepNamGrib2Surf;
    ncepHrrrSurfInitialization ncepHrrrSurf;

    NomadsWxModel nomad;

    VSIStatBufL sStat;
    VSIStatL( fileName.c_str(), &sStat );
    if( !VSI_ISDIR( sStat.st_mode ) && !strstr( fileName.c_str(), ".zip" ) )
    {
        //Determine what type of weather model the file came from
        //Check netcdf first, otherwise grib_api spews out crap
        if(ncepNdfd.identify(fileName)) {
            ncepNdfd.setModelFileName( fileName );
            return new ncepNdfdInitialization(ncepNdfd);
        }
        else if(ncepNamSurf.identify(fileName)) {
            ncepNamSurf.setModelFileName( fileName );
            return new ncepNamSurfInitialization(ncepNamSurf);
        }
        else if(ncepRapSurf.identify(fileName)) {
            ncepRapSurf.setModelFileName( fileName );
            return new ncepRapSurfInitialization(ncepRapSurf);
        }
        else if(ncepDgexSurf.identify(fileName)) {
            ncepDgexSurf.setModelFileName( fileName );
            return new ncepDgexSurfInitialization(ncepDgexSurf);
        }
        else if(ncepNamAlaskaSurf.identify(fileName)) {
            ncepNamAlaskaSurf.setModelFileName( fileName );
            return new ncepNamAlaskaSurfInitialization(ncepNamAlaskaSurf);
        }
        else if(ncepGfsSurf.identify(fileName)) {
            ncepGfsSurf.setModelFileName( fileName );
            return new ncepGfsSurfInitialization(ncepGfsSurf);
        }
        else if(wrfSurf.identify(fileName)) {
            wrfSurf.setModelFileName( fileName );
            CPLDebug("WX_MODEL_INITIALIZATION", "wrfSurf.identify(fileName) = %i\n", wrfSurf.identify(fileName));
            return new wrfSurfInitialization(wrfSurf);
        }
        #ifdef STABILITY
        else if(wrf3d.identify(fileName)) {    //fix this later to read either wrf3d or wrfSurf
            wrf3d.setModelFileName( fileName );
            CPLDebug("WX_MODEL_INITIALIZATION", "wrf3d.identify(fileName) = %i\n", wrf3d.identify(fileName));
            return new wrf3dInitialization(wrf3d);
        }
        #endif
        else if(ncepNamGrib2Surf.identify(fileName)) {
            ncepNamGrib2Surf.setModelFileName( fileName );
            return new ncepNamGrib2SurfInitialization(ncepNamGrib2Surf);
        }
        else if(ncepHrrrSurf.identify(fileName)) {
            ncepHrrrSurf.setModelFileName( fileName );
            return new ncepHrrrSurfInitialization(ncepHrrrSurf);
        }
        else if(genericSurf.identify(fileName)) {
            genericSurf.setModelFileName( fileName );
            return new genericSurfInitialization(genericSurf);
        }
        else {
            std::ostringstream outString;
            outString << "The weather model initialization file:\n  " << fileName << "\nCannot be " \
                    "identified as a valid weather model initialization file.";
            throw std::runtime_error(outString.str());
        }
    }
    else
    {
        if(nomad.identify(fileName))
        {
            const char **ppszKey = nomad.FindModelKey( fileName.c_str() );
            if( ppszKey )
                return new NomadsWxModel( fileName );
            else
            {
                std::ostringstream outString;
                outString << "The weather model initialization file:\n  " << fileName << "\nCannot be " \
                        "identified as a valid weather model initialization file.";
                throw std::runtime_error(outString.str());
            }
        }
        else {
            std::ostringstream outString;
        outString << "The weather model initialization file:\n  " << fileName << "\nCannot be " \
                "identified as a valid weather model initialization file.";
        throw std::runtime_error(outString.str());
        }
    }
}


