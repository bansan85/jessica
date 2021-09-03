import { TestBed } from '@angular/core/testing';

import { LibJessicaService } from './jessica.service';

describe('LibJessicaService', () => {
  let service: LibJessicaService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(LibJessicaService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
