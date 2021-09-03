import { TestBed } from '@angular/core/testing';

import { JessicaWasmService } from './jessica-wasm.service';

describe('JessicaWasmService', () => {
  let service: JessicaWasmService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(JessicaWasmService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
