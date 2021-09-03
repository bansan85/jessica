import { TestBed } from '@angular/core/testing';

import { MeyerhofCalcService } from './meyerhof-calc.service';

describe('MeyerhofCalcService', () => {
  let service: MeyerhofCalcService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(MeyerhofCalcService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
