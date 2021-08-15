#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_EPLUS()
{
	Identifier = "DEFAULT_PT_EPLUS";
	Name = "e+";
	Colour = PIXPACK(0xDFEFFF);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 1.00f;
	Collision = -0.99f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = -1;

	DefaultProperties.temp = R_TEMP + 200.0f + 273.15f;
	HeatConduct = 251;
	Description = "Positrons. React violently with electrons";

	Properties = TYPE_ENERGY|PROP_LIFE_DEC|PROP_LIFE_KILL_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;
	Create = &create;
}

static int update(UPDATE_FUNC_ARGS)
{
	int r, rt, rx, ry, nb, rrx, rry;
	for (rx=-2; rx<=2; rx++)
		for (ry=-2; ry<=2; ry++)
			if (BOUNDS_CHECK) {
				r = pmap[y+ry][x+rx];
				if (!r)
					r = sim->photons[y+ry][x+rx];
				if (!r)
					continue;
				rt = TYP(r);
				switch (rt)
				{
				case PT_ELEC:	
					int r, rx, ry, rt;
						for (rx=-1; rx<2; rx++)
							for (ry=-1; ry<2; ry++)
								if (BOUNDS_CHECK && (rx || ry))
								{
									r = pmap[y+ry][x+rx];
									if (!r)
										continue;
									rt = TYP(r);
									parts[i].life++;
									if (parts[i].life==4)
									{
										sim->kill_part(i);
										return 1;
									}
									
									sim->create_part(ID(r), x+rx, y+ry, PT_PHOT);
									
									sim->kill_part(ID(r));
									sim->pv[y/CELL][x/CELL] += 10.0f;
								}
				}
			}
	return 0;


static int graphics(GRAPHICS_FUNC_ARGS)
{
	*firea = 70;
	*firer = *colr;
	*fireg = *colg;
	*fireb = *colb;

	*pixel_mode |= FIRE_ADD;
	return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS)
{
	float a = RNG::Ref().between(0, 359) * 3.14159f / 180.0f;
	sim->parts[i].life = 680;
	sim->parts[i].vx = 2.0f * cosf(a);
	sim->parts[i].vy = 2.0f * sinf(a);
}
