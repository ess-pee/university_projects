function OUTPUT = ACCUMULATOR(INPUT)

HOUR = unique(INPUT(:,1));
WKDNO = INPUT(:,[1 2]);
WKDCO = INPUT(:,[1 3]);
WKDTR = INPUT(:,[1 4]);

WKDNO = accumarray(WKDNO(:,1),WKDNO(:,2),[],@mean);
WKDCO = accumarray(WKDCO(:,1),WKDCO(:,2),[], @mean);
WKDTR = accumarray(WKDTR(:,1),WKDTR(:,2),[], @median);

OUTPUT = [HOUR, WKDNO, WKDCO, WKDTR];

end