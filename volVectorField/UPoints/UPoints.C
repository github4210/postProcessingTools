/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Application
    UPoints

Description
    writes the components of velocity at each x, y, and z location

\*---------------------------------------------------------------------------*/

#include "calc.H"
#include "fvc.H"
#include <fstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::calc(const argList& args, const Time& runTime, const fvMesh& mesh)
{

    IOobject Uheader
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
    );

    if (Uheader.headerOk())
    {
	Info<<"Printing U component point data to U"<<runTime.timeName()<<".dat file \n";
	std::ofstream dataFile;
	string fileName;
	fileName ="U"+ runTime.timeName() +".dat";
	dataFile.open(fileName.c_str());//open the data file
        int cellCount=0;
	int boundaryFaceCount = 0;

        dataFile<< "% x\t y\t z\t Ux\t Uy\t Uz\t boundary " << "\n";
        volVectorField U(Uheader, mesh);
	const volVectorField& centers = mesh.C();
	const surfaceVectorField& faceCenters = mesh.Cf();

	// print all face values
	forAll(U.boundaryField(),patchi){
	
	label nFaces = mesh.boundaryMesh()[patchi].size();
        if(!isA<emptyPolyPatch>(mesh.boundaryMesh()[patchi])){
			for(int facei = 0; facei<nFaces; facei++){
			dataFile<<" "<<faceCenters.boundaryField()[patchi][facei].x()<<"\t "<<faceCenters.boundaryField()[patchi][facei].y()<<"\t "<<faceCenters.boundaryField()[patchi][facei].z()<<"\t "<<U.boundaryField()[patchi][facei].x()<<"\t "<<U.boundaryField()[patchi][facei].y()<<"\t "<<U.boundaryField()[patchi][facei].z()<<"\t 1\n";
			boundaryFaceCount++;
			}
		}	
	}
	Info<<"\t wrote face data from "<<boundaryFaceCount<<" boundary faces"<<endl;

	// print the internal field to the file
	forAll(U, cellI){

	dataFile<<" "<<centers[cellI].x()<<"\t "<<centers[cellI].y()<<"\t "<<centers[cellI].z()<<"\t "<<U[cellI].x()<<"\t "<<U[cellI].y()<<"\t "<<U[cellI].z()<<"\t 0\n";

	cellCount++;

	}

	Info<<"\t wrote cell data from "<<cellCount<<" cells"<<endl;

	dataFile.close();//close the data file

    }
    else
    {
        Info<< "    No U" << endl;
    }

    Info<< "\nEnd\n" << endl;
}


// ************************************************************************* //
