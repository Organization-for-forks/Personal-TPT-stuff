#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_AMTR()
{
	Identifier = "DEFAULT_PT_AMTR";
	Name = "AMTR";
	Colour = PIXPACK(0x808080);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.7f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.96f;
	Loss = 0.80f;
	Collision = 0.00f;
	Gravity = 0.10f;
	Diffusion = 1.00f;
	HotAir = 0.0000f * CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	DefaultProperties.tmp = 1; //tmp hack to preserve functionality on old saves (No longer used, I just gave up on the stuff i made, please help)

	Weight = 100;

	HeatConduct = 70;
	Description = "Anti-Matter, destroys a majority of particles.";

	Properties = TYPE_GAS;

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
	if (parts[i].tmp == 1)
	{
		int r, rx, ry, rt;
		for (rx=-1; rx<2; rx++)
			for (ry=-1; ry<2; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y+ry][x+rx];
					if (!r)
						continue;
					rt = TYP(r);
					int immune = parts[i].ctype;
					if (rt!=PT_AMTR && rt!=PT_DMND && rt!=PT_CLNE && rt!=PT_PCLN && rt!=PT_VOID && rt!=PT_BHOL && rt!=PT_NBHL && rt!=PT_PRTI && rt!=PT_PRTO && rt!=immune)
					{
					
						if (RNG::Ref().chance(1, 10))
							sim->create_part(ID(r), x+rx, y+ry, PT_PHOT);
						if (RNG::Ref().chance(19, 20))
							sim->kill_part(ID(r));
						sim->pv[y/CELL][x/CELL] -= 2.0f;
						sim->hv[y/CELL][x/CELL] = MAX_TEMP;
						sim->kill_part(i);
						return 1;
					}
				}
		return 0;
	}

	int r, rx, ry, rt;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				rt = TYP(r);
				int immune = parts[i].ctype;
				if (rt!=PT_AMTR && rt!=PT_DMND && rt!=PT_CLNE && rt!=PT_PCLN && rt!=PT_VOID && rt!=PT_BHOL && rt!=PT_NBHL && rt!=PT_PRTI && rt!=PT_PRTO && rt!=immune)
				{
					parts[i].life++;
					if (parts[i].life==4)
					{
						sim->kill_part(i);
						return 1;
					}
					if (RNG::Ref().chance(1, 10))
						sim->create_part(ID(r), x+rx, y+ry, PT_PHOT);
					else
						sim->kill_part(ID(r));
					sim->pv[y/CELL][x/CELL] -= 2.0f;
				}
			}
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	// don't render AMTR as a gas
	// this function just overrides the default graphics
	return 1;
}

static void create(ELEMENT_CREATE_FUNC_ARGS)
{
	sim->parts[i].tmp = 1;
}
