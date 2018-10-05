# monotop
* See the paper: [A Simple Testable Model of Baryon Number Violation: Baryogenesis,
Dark Matter, Neutron-Antineutron Oscillation and Collider Signals](https://arxiv.org/pdf/1712.02713.pdf)
* Get the model here:
http://uregina.ca/~kolev20n/monotop.html


##### In the following guide, we will generate Feynman diagrams for a monojet process, simulate proton-proton collisions in the CMS detector using the given BSM interactions, and analyze the data with ROOT.

### (1) MadGraph: Feynman diagram generation

We want to generate the following Feynman Diagram (Figure 1):


The syntax in MG5 is:
```
MG5_aMC> import model BaryogenX2N1Maj_withLeft
MG5_aMC> define bot = b b~
MG5_aMC> generate p p > n j bot
MG5_aMC> output monojet_with_assoc_btag
MG5_aMC> exit
```

This creates a folder called `monojet_with_assoc_btag` containing all the SubProcesses, Cards, and binaries needed for this process. In MG5, the proton carries gluons in its multiparticle definition by default, so you can check that we should have Figure 1 among the diagrams generated (you can `index.html` in a browser, for instance, with `firefox index.html`).


### (2) Staging the Event Generation: Cards
The first step to take before using MadEvent to simulate LHC collision data using our imported model and generated diagrams is to set the configurations; the configuration for each stage of simulation (MadEvent matrix element Monte Carlo > Pythia shower > Delphes detector simulation) is controlled by a __Card__. All the cards can be read and edited in the `monojet_with_assoc_btag/Cards` folder.

There are primarily 5 Cards to be aware of:

1. The __Process Card__ `proc_card_mg5.dat` - Leave as is; you already wrote this card when you did `generate` and `output` commands in the previous step.
2. The __Run Card__ `run_card.dat` - This Card controlls many things, but primarily you'll want to change

*  `nevents`: Number of collision events. By default `10000 = nevents` which should be a decent choice for now, but as you get deeper into the pheno study you may demand more statistics.

*  `use_syst`: change to `False = use_syst` since systematics calculations are initially unnecessary.

3. The __Pythia Card__ `pythia8_card.dat` (initially `pythia8_card_default.dat`) - Controlls the parameters of the particle shower. Typically this card only gets edited if you need to perform jet matching.
4. The __Delphes Card__ `delphes_card_CMS.dat` - Leave as-is, but have a peek inside - it is educational to see what parameters of the reconstruction are controlled here.
5. The __Param Card__: This is where we get to mess around with the laws of physics. All the masses, decay widths, and coupling constants of the Standard Model particles are set in this Card, and if you generated a BSM process by importing a different model, the free parameters of that theory will appear in this Card as well.

For the Baryogenesis model we're using, we should fix some masses and couplings first;

1. Set the mass of the second scalar X2 to be very large; effectively this will suppress interactions involving X2 to keep the phenomenology simple (and large mass gap is what we would expect from a symmetry breaking perspective anyways). For example, set MX1 = 1 TeV and MX2 = 10 TeV:

```
 6000001 1.000000e+03 # MX1
 6000002 1.000000e+04 # MX2
```
2. Turn on only the righthanded model as follows, controlled by these parameters (0 = OFF):

```
    1 1.000000e-01 # lam1Norm
    2 1.000000e-01 # lam2Norm
    3 0.000000e+00 # lam2Left << This is the coupling for the other helicity, like lam2Norm. Same flavor struture.
```


3. Set the couplings for the ddX Yukawa term as follows, only allowing down-bottom (i=1, j=3) interactions;

```
6 0.000000e+00 # lam1R113
```
```
15 0.000000e+00 # lam1R213
```
4. Next, set the couplings for the unX Yukawa term such that we only have i=1,2 (up, charm) interactions; this keeps the quarks pair produced with the dark matter light so as to not confuse them with our associated bottom jet in the process.
```
   40 1.000000e+00 # lam2R11
   41 1.000000e+00 # lam2R12
   42 0.000000e+00 # lam2R13
   43 1.000000e+00 # lam2R21
   44 1.000000e+00 # lam2R22
```

5. Set all the other couplings to zero. You can compare with the premade card link to make sure you set everything correctly.

Now you should be ready to generate your events.

### (3) Event Generation: MadEvent->Pythia8->Delphes
Every time you do this step, MadEvent will use the configured cards in the /Cards/ folder as they are; so, the only time you will have to go back to step 2 is if you want to edit some of the model parameters, in which case you will have to regenerate your events.

### (4) ROOT Data Analysis
