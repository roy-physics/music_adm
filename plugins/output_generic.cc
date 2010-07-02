/*
 
 output_generic.cc - This file is part of MUSIC -
 a code to generate multi-scale initial conditions 
 for cosmological simulations 
 
 Copyright (C) 2010  Oliver Hahn
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */


#ifdef HAVE_HDF5

#include "output.hh"
#include "HDF_IO.hh"


class generic_output_plugin : public output_plugin
{
protected:
	
	using output_plugin::cf_;
		
	template< typename Tt >
	void write2HDF5( std::string fname, std::string dname, const MeshvarBnd<Tt>& data )
	{
		int nres = data.size(0), nb = data.m_nbnd;
		std::vector<Tt> vdata;
		vdata.reserve((unsigned)pow(nres+2*nb,3));
		for(int i=-nb; i<nres+nb; ++i )
			for(int j=-nb; j<nres+nb; ++j )
				for(int k=-nb; k<nres+nb; ++k )
					vdata.push_back( data(i,j,k) );
		
		unsigned nd[3] = { nres+2*nb,nres+2*nb,nres+2*nb	};
		HDFWriteDataset3D( fname, dname, nd, vdata);
		
	}
	
public:
	generic_output_plugin( config_file& cf )//std::string fname, Cosmology cosm, Parameters param )
	: output_plugin( cf )//fname, cosm, param )
	{

		HDFCreateFile(fname_);
		
		HDFCreateGroup(fname_, "header");

		HDFWriteDataset(fname_,"/header/grid_off_x",offx_);
		HDFWriteDataset(fname_,"/header/grid_off_y",offy_);
		HDFWriteDataset(fname_,"/header/grid_off_z",offz_);
		
		HDFWriteDataset(fname_,"/header/grid_len_x",sizex_);
		HDFWriteDataset(fname_,"/header/grid_len_y",sizey_);
		HDFWriteDataset(fname_,"/header/grid_len_z",sizez_);
		
		HDFWriteGroupAttribute(fname_, "header", "levelmin", levelmin_ );
		HDFWriteGroupAttribute(fname_, "header", "levelmax", levelmax_ );
	}
	
	~generic_output_plugin()
	{	}
	
	void write_dm_mass( const grid_hierarchy& gh )
	{	}
	
	void write_dm_velocity( int coord, const grid_hierarchy& gh )
	{
		char sstr[128];
		
		for( unsigned ilevel=0; ilevel<=levelmax_; ++ilevel )
		{
			if( coord == 0 )
				sprintf(sstr,"level_%03d_DM_vx",ilevel);
			else if( coord == 1 )
				sprintf(sstr,"level_%03d_DM_vy",ilevel);
			else if( coord == 2 )
				sprintf(sstr,"level_%03d_DM_vz",ilevel);
			
			write2HDF5( fname_, sstr, *gh.get_grid(ilevel) );
		}
	}
	
	void write_dm_position( int coord, const grid_hierarchy& gh )
	{
		char sstr[128];
		
		for( unsigned ilevel=0; ilevel<=levelmax_; ++ilevel )
		{
			if( coord == 0 )
				sprintf(sstr,"level_%03d_DM_dx",ilevel);
			else if( coord == 1 )
				sprintf(sstr,"level_%03d_DM_dy",ilevel);
			else if( coord == 2 )
				sprintf(sstr,"level_%03d_DM_dz",ilevel);
			
			write2HDF5( fname_, sstr, *gh.get_grid(ilevel) );
		}
	}
	
	void write_dm_density( const grid_hierarchy& gh )
	{
		char sstr[128];
		
		for( unsigned ilevel=0; ilevel<=levelmax_; ++ilevel )
		{
			sprintf(sstr,"level_%03d_DM_rho",ilevel);
			write2HDF5( fname_, sstr, *gh.get_grid(ilevel) );
		}
	}
	
	void write_dm_potential( const grid_hierarchy& gh )
	{ 
		char sstr[128];
		
		for( unsigned ilevel=0; ilevel<=levelmax_; ++ilevel )
		{
			sprintf(sstr,"level_%03d_DM_potential",ilevel);
			write2HDF5( fname_, sstr, *gh.get_grid(ilevel) );
		}
	}
	
	void write_gas_potential( const grid_hierarchy& gh )
	{ 
		char sstr[128];
		
		for( unsigned ilevel=0; ilevel<=levelmax_; ++ilevel )
		{
			sprintf(sstr,"level_%03d_BA_potential",ilevel);
			write2HDF5( fname_, sstr, *gh.get_grid(ilevel) );
		}
	}
	
	
	
	void write_gas_velocity( int coord, const grid_hierarchy& gh )
	{	
		char sstr[128];
		
		for( unsigned ilevel=0; ilevel<=levelmax_; ++ilevel )
		{
			if( coord == 0 )
				sprintf(sstr,"level_%03d_BA_vx",ilevel);
			else if( coord == 1 )
				sprintf(sstr,"level_%03d_BA_vy",ilevel);
			else if( coord == 2 )
				sprintf(sstr,"level_%03d_BA_vz",ilevel);
			
			write2HDF5( fname_, sstr, *gh.get_grid(ilevel) );
		}
	}
	
	void write_gas_position( int coord, const grid_hierarchy& gh )
	{	}
	
	void write_gas_density( const grid_hierarchy& gh )
	{	
		char sstr[128];
		
		for( unsigned ilevel=0; ilevel<=levelmax_; ++ilevel )
		{
			sprintf(sstr,"level_%03d_BA_rho",ilevel);
			write2HDF5( fname_, sstr, *gh.get_grid(ilevel) );
		}
	}
	
	void finalize( void )
	{	}
};



namespace{
	output_plugin_creator_concrete< generic_output_plugin > creator("generic");
}


#endif

