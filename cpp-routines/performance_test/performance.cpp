/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "performance.h"
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
	int data;
	std::string line;
	multidet multi(1.19815, -17.2263);
	
	std::ifstream filei;
	std::ofstream fileo;	
	filei.open("performance.txt");
	fileo.open("output.txt");
	
	while (getline(filei, line))
	{
		data = std::stoi(line);
		data = multi.distribute(data);
		fileo<<data<<"\n";	
	}
	
	filei.close();
	fileo.close();
	return 0;
}

