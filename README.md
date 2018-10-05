# monotop


### MadGraph: Feynman diagram generation

We want to generate the following Feynman Diagram (Figure 1):


The syntax in MG5 is:
```
MG5_aMC> import model Baryogen
MG5_aMC> define bot = b b~
MG5_aMC> generate p p > n j bot
MG5_aMC> output monojet_with_assoc_btag
MG5_aMC> exit
```

This creates a folder called `monojet_with_assoc_btag` containing all the SubProcesses, Cards, and binaries needed for this process. In MG5, the proton carries gluons in its multiparticle definition by default, so you can check that we should have Figure 1 among the diagrams generated (you can `index.html` in a browser, for instance, with `firefox index.html`).


### Staging the Event Generation: Cards
The first step to take before using MadEvent to simulate LHC collision data using our imported model and generated diagrams is to set the configurations; the configuration for each stage of simulation (MadEvent matrix element Monte Carlo > Pythia shower > Delphes detector simulation) is controlled by a __Card__. All the cards can be read and edited in the `monojet_with_assoc_btag/Cards` folder.

There are primarily 4 Cards to be aware of:

1. The __Process Card__ `proc_card_mg5.dat` - Leave as is; you already wrote this card when you did `generate` and `output` commands in the previous step.
2. The __Run Card__ `run_card.dat` - This Card controlls many things, but primarily you'll want to change

*  `nevents`: Number of collision events. By default `10000 = nevents` which should be a decent choice for now, but as you get deeper into the pheno study you may demand more statistics.

*  `use_syst`: change to `False = use_syst` since systematics calculations are initially unnecessary.

3. The __Pythia Card__ `pythia8_card.dat` (initially `pythia8_card_default.dat`) - Controlls the parameters of the particle shower. Typically this card only gets edited if you need to perform jet matching.
4. The __Delphes Card__ `delphes_card_CMS.dat` - Leave as-is, but have a peek inside - it is educational to see what parameters of the reconstruction are controlled here.
