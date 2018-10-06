# monojet @ the LHC: A Baryon Number Violating Model
* See the paper: [A Simple Testable Model of Baryon Number Violation: Baryogenesis,
Dark Matter, Neutron-Antineutron Oscillation and Collider Signals](https://arxiv.org/pdf/1712.02713.pdf)
* Get the model here:
http://uregina.ca/~kolev20n/monotop.html

<p align="center">
  <img src="https://github.com/athompson-tamu/monojet/blob/master/images/lagrangian.PNG">
</p>

##### In the following guide, we will generate Feynman diagrams for a monojet process, simulate proton-proton collisions in the CMS detector using the given BSM interactions, and analyze the data with ROOT.

### (1) MadGraph: Drawing the Feynman Diagrams

Using the model linked above, we want MG5 to draw all the feynman diagrams for the final state (*N*,*u*,*b*) where *N* is the dark matter (DM), *u* is a light jet, and *b* is a BTagged jet. We specifically want the following diagram to be the dominate production mode among the diagrams generated:

<p align="center">
  <img src="https://github.com/athompson-tamu/monojet/blob/master/images/monojet_feyndiagram.png">
</p>
(Figure 1)


The best representative syntax in MG5 for the diagram in Figure 1 is:
```
MG5_aMC> import model BaryogenX2N1Maj_withLeft
MG5_aMC> define bot = b b~
MG5_aMC> define x = x1 x1~ x2 x2~
MG5_aMC> generate p p > x bot, x > n j
MG5_aMC> output baryogen_monojet_with_b
MG5_aMC> exit
```

This creates a folder called `baryogen_monojet_with_b` containing all the materials for this process. Enter the folder.

```
cd baryogen_monojet_with_b/
```


### (2) Prepping the Event Generation: Cards
```
cd Cards/
ls
```
Before we run MadEvent to simulate collision data using our generated diagrams, we have to configure various things. The set of configuration parameters for each stage of simulation is controlled by a __Card__. All the cards can be read and edited in the `baryogen_monojet_with_b/Cards` folder.

There are two primary Cards we want to edit, `run_card.dat` and `param_card.dat`. `pythia8_card.dat` and `delphes_card_CMS.dat` are worth having a look at sometime, as they can be relevant for other pheno work.

#### Run Card `run_card.dat`
This Card controls a broad range of things. The two parameters that you'll most often want to change from their defaults are:

1.  `nevents`: Number of collision events. By default `10000 = nevents` which should be a decent choice for now, but as you get deeper into the pheno study you may demand more statistics.

2. `use_syst`: change to `False = use_syst` since systematics calculations are initially unnecessary.

#### Param Card `param_card.dat`
This is where we get to mess around with the laws of physics. All the masses, decay widths, and coupling constants of the Standard Model particles are set in this Card, and if you generated a BSM process by importing a different model, the free parameters of that theory will appear in this Card as well. For the Baryogenesis model we're using, we should fix some masses and couplings first;

1. Set the mass of the second scalar X2 to be very large; effectively this will suppress interactions involving X2 to keep the phenomenology simple (and large mass gap is what we would expect from a symmetry breaking perspective anyways). For example, set MX1 = 1 TeV and MX2 = 10 TeV:

```
  5000001 1.000000e+00 # Mn
  6000001 1.000000e+03 # MX1
  6000002 2.000000e+04 # MX2
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

5. Set all the other couplings to zero. You can compare with [this premade card](https://github.com/athompson-tamu/monojet/blob/master/cards/param_card_right_mod.dat) to make sure you set everything correctly.

Now you should be ready to generate your events.

### (3) Event Generation: MadEvent->Pythia8->Delphes
Every time you do this step, MadEvent will use the cards in the /Cards/ folder as they are currently saved; so just make sure they are edited the way you want them to be each time MadEvent runs.

1. Launch Madevent:
```
cd ../
./bin/madevent
```
2. The MadEvent interface will show up, and to begin just type `launch <run_name>`;
```
baryogen_monojet_with_b> launch dryrun
```
Two prompts will show up. The first prompt asks which programs you would like to run; select Pythia8 and Delphes. The second prompt asks you if you want to edit your Cards any further; if you already edited them in Step __(2)__ then go ahead and press enter - let the event generation begin!

For this process, 10k events took me less than 10 minutes. Once Delphes has finished and the interface command prompt pops up again, you can exit with `baryogen_monojet_with_b> exit`.

The simulated data for each run is stored in the `Events/` folder:
```
cd Events/dryrun/
ls
```
* `tag_1_delphes_events.root` : this is the RECO-level ROOT file. Particles have been showered by Pythia and reconstructed with detector response. This data is like what a real detector would see.
* `unweighted_events.lhe.gz` : GEN-level ROOT file. Particles have not yet been showered yet, just the MadEvent output. This data is what we look at less often to get a theoretical "ground-truth" picture before final state particles decay.

### (4) ROOT Data Analysis

